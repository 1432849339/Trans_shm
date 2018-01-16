#ifndef _TRANS_H__
#define _TRANS_H__
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>
#include <memory>
#include <vector>
#include <list>
#include <fstream>
#include <deque>
#include <system_error>
#include <sys/stat.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <time.h>
#include <set>
#include <cstring>
#include <typeinfo>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <dirent.h>
#include <cstdint>
#include <unistd.h>
#include <utility>

#include "gflags/gflags.h"
#include "gloglinux/glog/logging.h"
#include "Struct/shm.h"
#include "Struct/quote-1017-lxl.h"
#include "GetUkey.h"
#include "Struct/SDS20Struct-2017.h"
#include "PathHelper.h"

#define CACHE_SIZE   100000
DECLARE_int32(endHour);
DECLARE_int32(endMinu);
DECLARE_string(data_dir);


using namespace std;
using namespace chronos;

struct FILEHEAD {
	char maintype[32];
	int  main_shmid;
	int  data_shmid;
	int  bytes_per_record;
	int  recnum_per_block;
	int  updatetm;
	int  recnum;
	char flags[4]; // 固定为shm //
	int  version;  // 目前为101 //
};

int64_t GetMsTime(int64_t ymd, int64_t hmsu);
time_t	GetTr_time();
int		GetTrday();
string	GetTime();
void	is_next_day();
void	Snapshot2str(Snapshot *ptr, string &str);
void	Order2str(Order *ptr, string &str);
void	Trans2str(Transaction* ptr, string &str);
void	Orderque2str(OrderQueue *ptr, string &str);

void	SH_IndexCodeTrans(char *ChCode);
bool	LvtToSnapshot(Snapshot& OutPut, SDS20LEVEL2& Input);
bool	IdxToSnapshot(Snapshot& OutPut, SDS20INDEX& InPut);
bool	Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut);
bool	TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut);
bool	OrdToOrder(Order& OutPut, SDS20ORDER& InPut);
bool	OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut);

bool	InitAndOpenToRead(char* shm_name, int& handle);

template<typename T>
bool shm_to_request(map<int, T>& data, 
					string& date, 
					function<void(T*, string&)>ToStr)
{
	static map<int64_t, path>	has_read_map;
	static map<string, string>	Request_FileName
	{
		{ "N7chronos10OrderQueueE", "OrderQueue.csv" },
		{ "N7chronos11TransactionE", "Transaction.csv" },
		{ "N7chronos5OrderE", "Order.csv" },
		{ "N7chronos8SnapshotE", "Snapshot.csv" },
	};
	extern map<int, string> MarketID;
	extern map<int, string> VarID;

	int64_t ukey = (data.begin()->second).ukey;
	boost::filesystem::path request_name;
	std::ofstream fout_csv;
	if (has_read_map.find(ukey) == has_read_map.end())
	{
		request_name /= FLAGS_data_dir;
		int mk = 0, ty = 0;
		get_variety_market_by_ukey(ukey, ty, mk);
		if ((MarketID.find(mk) == MarketID.end()) || (VarID.find(ty) == VarID.end()))
		{
			return false;
		}
		request_name /= MarketID[mk];
		request_name /= VarID[ty];
		request_name /= itostr(ukey);
		request_name /= date.substr(0, 4);
		request_name /= date;
		while (!CreateDir(request_name.string()))
		{
			LOG(WARNING) << "等待创建文件夹 : " << request_name << " 成功...";
			this_thread::sleep_for(chrono::seconds(1));
		}
		request_name /= Request_FileName[typeid(T).name()];
		fout_csv.open(request_name.string());
		if (!fout_csv.is_open())
		{
			LOG(ERROR) << "create " << request_name << "  error";
			return false;
		}
		has_read_map[ukey] = request_name;
	}
	else
	{
		fout_csv.open(has_read_map[ukey].string(), ios::app);
		if (!fout_csv.is_open())
		{
			LOG(ERROR) << "create " << request_name << "  error";
			return false;
		}
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

template<typename INPUT, typename OUTPUT>
int	shm2file(string& shm_name, 
			 string& date, 
			 function<bool(OUTPUT&, INPUT&)> Trans, 
			 function<void(OUTPUT*, string&)> ToStr)
{
	extern bool flag_next;
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
	LOG(INFO) << "open " << shm_name_c << " seccuss!";

	INPUT temp{ 0 };
	auto temp_size = sizeof(temp);
	OUTPUT temp_snapshot{ 0 };
	auto temp_snapshot_size = sizeof(temp_snapshot);
	map<int64_t, map<int, OUTPUT>>  Cache;
	int Cache_size = 0;

	int i = 0;
	while (flag_next)
	{
		if (i < shmRecnum(handle))
		{
			i++;
			memset(&temp, 0, temp_size);
			memset(&temp_snapshot, 0, temp_snapshot_size);
			shmRead(handle, (char*)(&temp), i);
			//LOG(INFO) << "读取的内存块 : " << shm_name << " 当前位置 = " << i << " 现在最大的index = " << shmRecnum(handle);
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
					//LOG(INFO) << "写入数据ukey = " << it.first << " 写入index = " << i << "成功!";
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
					//LOG(INFO) << "写入数据ukey = " << it.first << " 写入index = " << i << "成功!";
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
	shmClose(shm_name_c);
	return 0;
}


int thread_shm_to_local();

#endif