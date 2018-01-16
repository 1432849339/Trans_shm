#include "gflags/gflags.h"
#include "GetUkey.h"


DEFINE_string(csvFilePath, "/home/sds/fqy/local_file", "文件登陆时,文件的目录位置");
DEFINE_string(mysql_url, "172.24.54.1", "数据库ip");
DEFINE_string(mysql_name, "ukdbr", "数据库账户名");
DEFINE_string(mysql_password, "ukdbr", "数据库密码");
DEFINE_string(mysql_database, "ukdb09", "数据库名");

chronos::SecurityMaster ukey_db;
map<std::string, int> g_market_ids =
{
	{ "SZ",  MARKET_SZA },
	{ "SH",  MARKET_SHA },
	{ "SZB", MARKET_SZB },
	{ "SHB", MARKET_SHB },
	{ "CF",  MARKET_CFE },
	{ "SHF", MARKET_SHF },
	{ "CZC", MARKET_CZC },
	{ "DCE", MARKET_DCE },
	{ "SGE", MARKET_SGE },
};

bool Init_ukdb09(const std::set<std::string>& market_set, bool use_file)
{
	std::cout << "InitUkeyDB start use_file=" << use_file << endl;
	int count = 0;
	if (use_file)
	{
		count = ukey_db.UKOpen(FLAGS_csvFilePath);
	}
	else
	{
		set<int> market_ids;
		bool market_error = false;
		if (market_set.size() == 0)
		{
			for (auto &p : g_market_ids)
			{
				market_ids.insert(p.second);
			}
		}
		else
		{
			auto it = g_market_ids.begin();
			for (auto &mk : market_set)
			{
				if ((it = g_market_ids.find(mk)) != g_market_ids.end())
				{
					market_ids.insert(it->second);
				}
				else
				{
					market_error = true;
					cerr << "Market ID is not defined market=" << mk << endl;
				}
			}
		}
		if (market_ids.size() == 0 || market_error)
		{
			cout << "Init Market ID error" << endl;
			return false;
		}
		for (auto mk : market_ids)
		{
			cout << "ukey_db market_id=" << mk << endl;
		}
		count = ukey_db.UKOpen(market_ids, FLAGS_mysql_url, FLAGS_mysql_name, FLAGS_mysql_password, FLAGS_mysql_database, 0);
	}
	cout << "InitUkeyDB done use_file=" << use_file << ", count=" << count << endl;
	return true;
}

int64_t	GetUkey(std::string market, std::string code)
{
	std::transform(market.begin(), market.end(), market.begin(), ::toupper);
	auto it = g_market_ids.find(market);
	if (it == g_market_ids.end())
	{
		cerr << "Market ID not defined market=" << market << ", code=" << code << endl;
		return 0;
	}

	string ucode = code;
	std::transform(ucode.begin(),ucode.end(), ucode.begin(), ::toupper);
	SecurityInfo info;
	if (market == "SZ" || market == "SH")
	{
		auto ret = ukey_db.GetBaseInfo(it->second, ucode, info);
		if (ret)
		{
			auto ret = ukey_db.GetBaseInfo(it->second + 7, ucode, info);
			if (ret)
			{
				return 0;
			}
		}
	}
	else
	{
		auto ret = ukey_db.GetBaseInfo(it->second, ucode, info);
		if (ret)
		{
			return 0;
		}
	}
	return info.ukey;
}