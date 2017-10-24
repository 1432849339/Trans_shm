#ifndef _TRANS_H__
#define _TRANS_H__
#include <cstdio>
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

using namespace std;
using namespace chronos;

int64_t GetMsTime(int64_t ymd, int64_t hmsu);
void LvtToSnapshot(Snapshot& OutPut, map<int64_t, OrderQueue>& AskQue, map<int64_t, OrderQueue>& BidQue, SDS20LEVEL2& Input);
void IdxToSnapshot(Snapshot& OutPut, SDS20INDEX& InPut);
void Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut);
void TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut);
void OrdToOrder(Order& OutPut, SDS20ORDER& InPut);
void OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut);

#endif