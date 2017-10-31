
#include "shm.h"
#include "Trans.h"
#include "SDS20Struct-2017.h"
#include "CreatePath.h"


using namespace std;

const string base_path= "/mnt/data/fqy/Request/";
bool   flag_next(true);

map<int, string> MarketID{
	// 亚太地区
	//{ MARKET_ALL, "ALL" },
	{ MARKET_SZA, "SZA" },
	{ MARKET_SHA, "SHA" },
	{ MARKET_CFX, "CFX" },
	{ MARKET_SHF, "SHF" },
	{ MARKET_CZC, "CZC" },
	{ MARKET_DCE, "DCE" },
	{ MARKET_SGE, "SGE" },
	{ MARKET_SZB, "SZB" },
	{ MARKET_SHB, "SHB" },
	{ MARKET_HK, "HK" },
	{ MARKET_IBBM, "IBBM" },
	{ MARKET_OTC, "OTC" },
	{ MARKET_TAIFEX, "TAIFEX" },
	{ MARKET_SGX, "SGX" },
	{ MARKET_SICOM, "SICOM" },
	{ MARKET_JPX, "JPX" },
	{ MARKET_TOCOM, "TOCOM" },
	{ MARKET_BMD, "BMD" },
	{ MARKET_TFEX, "TFEX" },
	{ MARKET_AFET, "AFET" },
	{ MARKET_KRX, "KRX" },


	// 欧洲地区
	{ MARKET_LME, "LME" },
	{ MARKET_ICE, "ICE" },
	{ MARKET_LIFFE, "LIFFE" },
	//{ MARKET_XEurex, "XEurex" },

	// 美洲地区
	{ MARKET_CME, "CME" },
	{ MARKET_CBOT, "CBOT" },
	{ MARKET_NYBOT, "NYBOT" },
	{ MARKET_NYMEX_COMEX, "NYMEX_COMEX" },
	{ MARKET_ICE_CANOLA, "ICE_CANOLA" },
	{ MARKET_eCBOT, "eCBOT" },
	{ MARKET_CBOE, "CBOE" },

	// 其他地区
	{ MARKET_SFE, "SFE" },
	{ MARKET_DME, "DME" },
	{ MARKET_DGCX, "DGCX" },
};

map<int, string> VarID{
	//{ VARIETY_ALL, "all" },
	{ VARIETY_STOCK, "stock" },
	{ VARIETY_BOND, "bond" },
	{ VARIETY_FUND, "fund" },
	{ VARIETY_SPOT, "spot" },
	{ VARIETY_MONEY_MARKET, "money" },
	{ VARIETY_INDEX, "index" },
	{ VARIETY_FUTURE, "future" },
	{ VARIETY_OPTION, "option" },
	{ VARIETY_WARRANT, "warrant" },
	{ VARIETY_STOCK_OPTION, "stcopt" },
};

template<typename T>
bool shm_to_request(map<int,T>& data,string& date,function<void(T*, string&)>ToStr)
{
	string request_name = base_path;
	int64_t ukey = (data.begin()->second).ukey;
	int mk = 0, ty = 0;
	get_variety_market_by_ukey(ukey, ty, mk);
	if ((MarketID.find(mk) != MarketID.end()) && (VarID.find(ty) != VarID.end()))
	{
		map<string, string>Request_FileName
		{
			{ "N7chronos10OrderQueueE", "OrderQueue.csv" },
			{ "N7chronos11TransactionE", "Transaction.csv" },
			{ "N7chronos5OrderE", "Order.csv" },
			{ "N7chronos8SnapshotE", "Snapshot.csv" },
		};
		request_name += MarketID[mk];
		request_name += "/";
		request_name += VarID[ty];
		request_name += "/";
		request_name += itostr(ukey);
		request_name += "/";
		request_name += date.substr(0, 4);
		request_name += "/";
		request_name += date;
		request_name += "/";
		while (!mkdir_path(request_name))
		{
			cout << "waitting mkdir " << request_name << endl;
		}
		request_name += Request_FileName[typeid(T).name()];
		ofstream fout_csv(request_name,ios::app);
		if (!fout_csv.is_open())
		{
			cerr << "create " << request_name << "  error" << endl;
			return false;
		}
		string str;
		for (auto &it : data)
		{
			str.clear();
			ToStr(&(it.second), str);
			fout_csv << str;
		}
		fout_csv.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool InitAndOpenToRead(char* shm_name,int& handle)
{
	int retcode = 0;
	char appname[] = "";
	retcode = shmInit(9901, appname);
	if (retcode != 0)
	{
		printf("\n shmInit error=[%d]   %s\n", retcode,GetTime().c_str());
		return false;
	}
	retcode = shmOpenForRead(shm_name);
	if (retcode != 0)
	{
		printf("\n shmOpen error=[%d]  %s  %s\n", retcode, shm_name, GetTime().c_str());
		return false;
	}
	handle = shmHandle(shm_name);
	int recnum = shmRecnum(handle);
	cout << "recnum=" << recnum << "\t"<< shm_name <<"\t"<< GetTime() << endl;
	return true;
}

template<typename INPUT,typename OUTPUT>
int shm2file(string& shm_name,string& file_name,string& date,function<bool(OUTPUT&,INPUT&)> Trans,function<void(OUTPUT*,string&)> ToStr)
{
	int handle = 0;
	const auto shm_name_size = shm_name.size() + 1;
	char shm_name_c[shm_name_size];
	strncpy(shm_name_c, shm_name.c_str(), shm_name_size);
	while (!InitAndOpenToRead(shm_name_c, handle))
	{
		if (!flag_next)
		{
			return 0;
		}
		this_thread::sleep_for(chrono::seconds(60));
	}
	cout << "open " << shm_name_c << " seccuss!!!" << "\t" << GetTime() << endl;

	INPUT temp{ 0 };
	auto temp_size = sizeof(temp);
	OUTPUT temp_snapshot{ 0 };
	auto temp_snapshot_size = sizeof(temp_snapshot);
	map<int64_t, map<int,OUTPUT>>  Cache;
	int Cache_size = 0;

	ofstream flog;
	flog.open(file_name+"_run.log");
	if (!flog.is_open())
	{
		cerr << "open " << file_name << " log error" << "\t" << GetTime() << endl;
		return -2;
	}
	int i = 0;
	while (flag_next)
	{
		if (i < shmRecnum(handle))
		{
			i++;
			memset(&temp, 0, temp_size);
			memset(&temp_snapshot, 0, temp_snapshot_size);
			shmRead(handle, (char*)(&temp), i);
			flog << "Current = " << i <<"       MAX = "<< shmRecnum(handle) << endl;
			flog.flush();
			if (Trans(temp_snapshot, temp))
			{
				Cache[temp_snapshot.ukey][i] = temp_snapshot;
				Cache_size++;
			}
			if (Cache_size > CACHE_SIZE)
			{
				for (auto &it : Cache)
				{
					shm_to_request<OUTPUT>(it.second, date, ToStr);
					flog << "更新数据 ukey = " << it.first << " 成功" << endl;
					flog << "Current = " << i << "        MAX = " << shmRecnum(handle) << endl;
					flog.flush();
				}
				Cache_size = 0;
				Cache.clear();
			}
		}
		else
		{
			if (Cache_size != 0)
			{
				for (auto &it : Cache)
				{
					shm_to_request<OUTPUT>(it.second, date, ToStr);
					flog << "更新数据 ukey = " << it.first << " 成功" << endl;
					flog << "Current = " << i << "        MAX = " << shmRecnum(handle) << endl;
					flog.flush();
				}
				Cache_size = 0;
				Cache.clear();
				this_thread::sleep_for(chrono::seconds(20));
			}
			else
			{
				this_thread::sleep_for(chrono::seconds(30));
			}
		}
	}
	flog.close();
	return 0;
}

void is_next_day()
{
	using std::chrono::system_clock;
	time_t tt = GetTr_time();
	struct std::tm * ptm = std::localtime(&tt);
	ptm->tm_hour = SPLIT_HROUS;
	ptm->tm_min = SPLIT_MINN;
	ptm->tm_sec = 0;
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
	flag_next = false;
}

int thread_shm_to_local(string& date)
{
	flag_next = true;
	string Path = "/mnt/data/fqy/run_log/";
	if (!CreatePath(Path))
	{
		cerr << "create Path error!" << endl;
		return -1;
	}
	set<string> market_set;
	if (!Init_ukdb09(market_set,true))
	{
		cerr << "init database ukdb09 error" << "\t" << GetTime() << endl;
		return -1;
	}
	list<std::shared_ptr<std::thread>> m_threadgroup;
	auto thread_cfe = [&]() {
		string shm_name = "CFE" + date;
		string file_name = Path + "Snapshot_cfe";
		shm2file<SDS20FUTURE, Snapshot>(shm_name, file_name,date, Cfe_SpiToSnapshot,Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_cfe));

	auto thread_spi = [&]() {
		this_thread::sleep_for(chrono::seconds(3));
		string shm_name = "SPI" + date;
		string file_name = Path + "Snapshot_spi";
		shm2file<SDS20FUTURE, Snapshot>(shm_name, file_name,date, Cfe_SpiToSnapshot,Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_spi));

	auto thread_lvt = [&]() {
		this_thread::sleep_for(chrono::seconds(6));
		string shm_name = "LVT" + date;
		string file_name = Path + "Snapshot_lvt";
		shm2file<SDS20LEVEL2, Snapshot>(shm_name, file_name,date, LvtToSnapshot,Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_lvt));

	auto thread_idx = [&]() {
		this_thread::sleep_for(chrono::seconds(9));
		string shm_name = "IDX" + date;
		string file_name = Path + "Snapshot_idx";
		shm2file<SDS20INDEX, Snapshot>(shm_name, file_name,date, IdxToSnapshot,Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_idx));

	auto thread_orq = [&]() {
		this_thread::sleep_for(chrono::seconds(12));
		string shm_name = "ORQ" + date;
		string file_name = Path + "OrderQueue";
		shm2file<SDS20ORDERQUEUE, OrderQueue>(shm_name, file_name,date, OrqToOrderqueue,Orderque2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_orq));

	auto thread_ord = [&]() {
		this_thread::sleep_for(chrono::seconds(15));
		string shm_name = "ORD" + date;
		string file_name = Path + "Order";
		shm2file<SDS20ORDER, Order>(shm_name, file_name,date, OrdToOrder,Order2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_ord));

	auto thread_trd = [&]() {
		this_thread::sleep_for(chrono::seconds(18));
		string shm_name = "TRD" + date;
		string file_name = Path + "Transaction";
		shm2file<SDS20TRANSACTION, Transaction>(shm_name, file_name, date, TrdToTransaction, Trans2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_trd));

	for (auto &it : m_threadgroup)
	{
		if (it->joinable())
		{
			it->join();
		}
	}
}

bool delete_history(string& BasePath,string& date)
{
	DIR *dir = nullptr;
	struct dirent *ptr = nullptr;
	char		windCode[32]{ 0 };

	if ((dir = opendir(BasePath.c_str())) == nullptr)
	{
		cerr << "BasePath is inexist" << endl;;
		return false;
	}
	while ((ptr = readdir(dir)) != nullptr)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)//隐藏文件
		{
			continue;
		}
		else if (ptr->d_type == 8)//普通文件
		{
			continue;
		}
		else if (ptr->d_type == 10)//链接
		{
			continue;
		}
		else if (ptr->d_type == 4)//目录
		{
			string Base;
			Base = BasePath;
			if (date == ptr->d_name && date.substr(0, 4) == Base.substr(Base.find_last_of('/')+1))
			{//找到历史数据文件夹,删除
				Base += "/";
				Base += ptr->d_name;
				if (rmdir(Base.c_str()) != 0)
				{
					cerr << "remove " << Base << " error!" << endl;
					//return false;
				}
			}
			else//没找到继续递归
			{
				Base += "/";
				Base += ptr->d_name;
				delete_history(Base, date);
			}
		}
	}
	closedir(dir);
	return true;
}

bool remove_history(string& Path)
{
	if (access(Path.c_str(), F_OK) != 0)
	{
		return true;
	}
	else
	{
		if (rmdir(Path.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

int main()
{
	while (true)
	{
		stringstream ss;
		ss << GetTrday();
		string date = ss.str();
		cout << "开始实时更新数据 : " << date << "\t" << GetTime() << endl;
		thread thread_trans(thread_shm_to_local, std::ref(date));
		thread thread_next(is_next_day);
		thread_trans.join();
		thread_next.join();
		cout << "实时数据生成成功 : " << date << "\t" << GetTime() << endl;
		this_thread::sleep_for(chrono::seconds(300));
	}
    return 0;
}