#ifndef _TRANS_H__
#define _TRANS_H__
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>
#include <map>
#include <algorithm>
#include <memory>
#include <vector>
#include <list>
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
#include "ukey.h"
#include "quote-1017-lxl.h"
#include "GetUkey.h"
#include "SDS20Struct-2017.h"

#define SPLIT_HROUS  19
#define SPLIT_MINN   0
#define CACHE_SIZE   100000

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
time_t GetTr_time();
int GetTrday();
string GetTime();
void Snapshot2str(Snapshot *ptr, string &str);
void Order2str(Order *ptr, string &str);
void Trans2str(Transaction* ptr, string &str);
void Orderque2str(OrderQueue *ptr, string &str);

void SH_IndexCodeTrans(char *ChCode);
//void LvtToSnapshot(Snapshot& OutPut, map<int64_t, OrderQueue>& AskQue, map<int64_t, OrderQueue>& BidQue, SDS20LEVEL2& Input);
bool LvtToSnapshot(Snapshot& OutPut, SDS20LEVEL2& Input);
bool IdxToSnapshot(Snapshot& OutPut, SDS20INDEX& InPut);
bool Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut);
bool TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut);
bool OrdToOrder(Order& OutPut, SDS20ORDER& InPut);
bool OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut);

#endif