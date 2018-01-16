#include "Trans.h"


extern string date;
bool   flag_next(true);
DEFINE_int32(endHour, 18, "停止时间hour");
DEFINE_int32(endMinu, 40, "停止时间minu");
DEFINE_string(data_dir, "./data", "数据存放目录");
DEFINE_string(LvtPrefix, "LVT", "LVT行情前缀");
DEFINE_string(IdxPrefix, "IDX", "IDX行情前缀");
DEFINE_string(SpiPrefix, "SPI", "SPI行情前缀");
DEFINE_string(CfePrefix, "CFE", "CFE行情前缀");
DEFINE_string(OrdPrefix, "ORD", "深圳逐笔委托");
DEFINE_string(OrqPrefix, "ORQ", "委托队列");
DEFINE_string(TrdPrefix, "TRD", "逐笔数据");

//交易时间分段
DEFINE_string(Ruler_SZ_SH, "[925-1130 1300-1505]", "SZ SH交易时间");
DEFINE_string(Ruler_CF, "[915-1130 1300-1515]", "中国金融交易所,交易时间");
DEFINE_string(Ruler_SHF, "[2100-2359 0-100 900-1130 1330-1505]", "上海期货交易所,交易时间");
DEFINE_string(Ruler_DCE, "[2100-2330 900-1130 1330-1505]", "大连商品期货交易所");
DEFINE_string(Ruler_CZC, "[2100-2330 900-1130 1330-1505]", "郑州期货交易所");


map<int, string> MarketID{
	// 亚太地区
	//{ MARKET_ALL, "ALL" },
	{ MARKET_SZA, "SZA" },
	{ MARKET_SHA, "SHA" },
	{ MARKET_CFE, "CFE" },
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

time_t GetTr_time()
{
	time_t tt;
	struct tm* pstm = nullptr;
	tt = time(nullptr);
	pstm = localtime(&tt);
	if ((pstm->tm_hour == FLAGS_endHour && pstm->tm_min >= FLAGS_endMinu) || (pstm->tm_hour > FLAGS_endHour))
	{
		tt += 1 * 24 * 60 * 60;
	}
	return tt;
}

string GetTime()
{
	time_t tt;
	struct tm*	pstm = nullptr;
	tt = time(nullptr);
	pstm = localtime(&tt);
	int year = pstm->tm_year + 1900;
	int mon = pstm->tm_mon + 1;
	int day = pstm->tm_mday;
	char time_c[32];
	sprintf(time_c, "%04d%02d%02d %02d:%02d:%02d", year, mon, day, pstm->tm_hour, pstm->tm_min, pstm->tm_sec);
	return time_c;
}

int GetTrday()
{
	time_t tt;
	struct tm*	pstm = nullptr;
	tt = time(nullptr);
	pstm = localtime(&tt);
	if((pstm->tm_hour == FLAGS_endHour && pstm->tm_min >= FLAGS_endMinu) || (pstm->tm_hour > FLAGS_endHour))
	{
		tt += 1 * 24 * 60 * 60;
	}
	pstm = localtime(&tt);
	int year = pstm->tm_year + 1900;
	int mon = pstm->tm_mon + 1;
	int day = pstm->tm_mday;
	return year * 10000 + mon * 100 + day;
}

void is_next_day()
{
	using std::chrono::system_clock;
	time_t tt = GetTr_time();
	struct std::tm * ptm = std::localtime(&tt);
	ptm->tm_hour = FLAGS_endHour;
	ptm->tm_min = FLAGS_endMinu;
	ptm->tm_sec = 0;
	std::this_thread::sleep_until(system_clock::from_time_t(mktime(ptm)));
	flag_next = false;
}

void Snapshot2str(Snapshot *ptr, string &str)
{
	stringstream ss;
	ss << ptr->ukey << ","
		<< ptr->trday << ","
		<< ptr->timeus << ","
		<< ptr->recvus << ","
		<< ptr->status << ","
		<< ptr->pre_close << ","
		<< ptr->high << ","
		<< ptr->low << ","
		<< ptr->open << ","
		<< ptr->last << ","
		<< ptr->match_num << ","
		<< ptr->volume << ","
		<< ptr->turnover << ",";

	for (int i = 0; i < 10; ++i)
		ss << ptr->info[i] << ",";
	for (int i = 0; i < 10; ++i)
		ss << ptr->ask_price[i] << ",";
	for (int i = 0; i < 10; ++i)
		ss << ptr->ask_volume[i] << ",";
	for (int i = 0; i < 10; ++i)
		ss << ptr->bid_price[i] << ",";
	for (int i = 0; i < 10; ++i)
		ss << ptr->bid_volume[i] << ",";
	ss << ptr->ask_orders_num << ","
		<< ptr->bid_orders_num << ",";
	for (int i = 0; i < 50; ++i)
		ss << ptr->ask_queue[i] << ",";
	for (int i = 0; i < 49; ++i)
		ss << ptr->bid_queue[i] << ",";
	ss << ptr->bid_queue[49] << endl;
	str = ss.str();
}

void Order2str(Order *ptr, string &str)
{
	stringstream ss;
	ss << ptr->ukey << ","
		<< ptr->trday << ","
		<< ptr->timeus << ","
		<< ptr->recvus << ","
		<< ptr->index << ","
		<< ptr->price << ","
		<< ptr->volume << ","
		<< ptr->order_type << endl;

	str = ss.str();
}

void Trans2str(Transaction* ptr, string &str)
{
	stringstream ss;
	ss << ptr->ukey << ","
		<< ptr->trday << ","
		<< ptr->timeus << ","
		<< ptr->recvus << ","
		<< ptr->index << ","
		<< ptr->price << ","
		<< ptr->volume << ","
		<< ptr->ask_order << ","
		<< ptr->bid_order << ","
		<< ptr->trade_type << endl;
	str = ss.str();
}

void Orderque2str(OrderQueue *ptr, string &str)
{
	stringstream ss;
	ss << ptr->ukey << ","
		<< ptr->trday << ","
		<< ptr->timeus << ","
		<< ptr->recvus << ","
		<< ptr->side << ","
		<< ptr->price << ","
		<< ptr->orders_num << ",";
	for (int i = 0; i < 49; ++i)
		ss << ptr->queue[i] << ",";
	ss << ptr->queue[49] << endl;
	str = ss.str();
}

void SH_IndexCodeTrans(char *ChCode)
{
	switch (atoi(ChCode))
	{
	case 999999:
		memcpy(ChCode, "000001", 6);
		break;
	case 999998:
		memcpy(ChCode, "000002", 6);
		break;
	case 999997:
		memcpy(ChCode, "000003", 6);
		break;
	case 999996:
		memcpy(ChCode, "000004", 6);
		break;
	case 999995:
		memcpy(ChCode, "000005", 6);
		break;
	case 999994:
		memcpy(ChCode, "000006", 6);
		break;
	case 999993:
		memcpy(ChCode, "000007", 6);
		break;
	case 999992:
		memcpy(ChCode, "000008", 6);
		break;
	case 999991:
		memcpy(ChCode, "000010", 6);
		break;
	case 999990:
		memcpy(ChCode, "000011", 6);
		break;
	case 999989:
		memcpy(ChCode, "000012", 6);
		break;
	case 999988:
		memcpy(ChCode, "000013", 6);
		break;
	case 999987:
		memcpy(ChCode, "000016", 6);
		break;
	case 999986:
		memcpy(ChCode, "000015", 6);
		break;
	default:
		if (strlen(ChCode) > 2)
			memcpy(ChCode, "00", 2);
		break;
	}
}

bool LvtToSnapshot(Snapshot& OutPut, SDS20LEVEL2& Input)
{
	string windcode = string(Input.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (ukey == 0)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(Input.nActionDay, Input.nTime);
	OutPut.recvus = (Input.nRecvTime < Input.nTime ? OutPut.timeus : GetMsTime(Input.nActionDay, Input.nRecvTime));
	OutPut.status = 0;
	OutPut.pre_close = Input.nPreClose;
	OutPut.high = Input.nHigh;
	OutPut.low = Input.nLow;
	OutPut.open = Input.nOpen;
	OutPut.last = Input.nMatch;
	OutPut.match_num = Input.nNumTrades;
	OutPut.volume = Input.iVolume;
	OutPut.turnover = Input.iTurnover;
	OutPut.info[0] = Input.nHighLimited;
	OutPut.info[1] = Input.nLowLimited;
	OutPut.info[2] = Input.nTotalBidVol;
	OutPut.info[3] = Input.nTotalAskVol;
	OutPut.info[4] = Input.nWeightedAvgBidPrice;
	OutPut.info[5] = Input.nWeightedAvgAskPrice;
	OutPut.info[6] = Input.nIOPV;
	OutPut.info[7] = Input.nYieldToMaturity;
	OutPut.info[8] = Input.nSyl1;
	OutPut.info[9] = Input.nSyl2;
	for (int i = 0; i < 10; ++i)
	{
		OutPut.ask_price[i] = Input.nAskPrice[i];
	}
	for (int i = 0; i < 10; ++i)
	{
		OutPut.ask_volume[i] = Input.nAskVol[i];
	}
	for (int i = 0; i < 10; ++i)
	{
		OutPut.bid_price[i] = Input.nBidPrice[i];
	}
	for (int i = 0; i < 10; ++i)
	{
		OutPut.bid_volume[i] = Input.nBidVol[i];
	}
	return true;
}

bool IdxToSnapshot(Snapshot& OutPut, SDS20INDEX& InPut)
{
	char   code[32]{ 0 };
	strncpy(code, InPut.szWindCode, 32);
	if (0 == strcasecmp(code + 7, "SH"))
	{
		SH_IndexCodeTrans(code);
	}
	string windcode = code;
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(InPut.nActionDay, InPut.nTime);
	OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(InPut.nActionDay, InPut.nRecvTime));
	OutPut.status = 0;
	OutPut.pre_close = InPut.nPreCloseIndex;
	OutPut.high = InPut.nHighIndex;
	OutPut.low = InPut.nLowIndex;
	OutPut.open = InPut.nOpenIndex;
	OutPut.last = InPut.nLastIndex;
	OutPut.match_num = 0;
	OutPut.volume = InPut.iTotalVolume;
	OutPut.turnover = InPut.iTurnover;
	return true;
}

bool Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(InPut.nActionDay, InPut.nTime);
	OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(InPut.nActionDay, InPut.nRecvTime));
	OutPut.status = 0;
	OutPut.pre_close = InPut.nPreClose;
	OutPut.high = InPut.nHigh;
	OutPut.low = InPut.nLow;
	OutPut.open = InPut.nOpen;
	OutPut.last = InPut.nMatch;
	OutPut.match_num = 0;
	OutPut.volume = InPut.iVolume;
	OutPut.turnover = InPut.iTurnover;
	OutPut.info[0] = InPut.nHighLimited;
	OutPut.info[1] = InPut.nLowLimited;
	OutPut.info[2] = InPut.nSettlePrice;
	OutPut.info[3] = InPut.nPreSettlePrice;
	OutPut.info[4] = InPut.iOpenInterest;
	OutPut.info[5] = InPut.iPreOpenInterest;
	OutPut.info[6] = InPut.nCurrDelta;
	OutPut.info[7] = InPut.nPreDelta;
	OutPut.info[8] = 0;
	OutPut.info[9] = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			OutPut.ask_price[i] = InPut.nAskPrice[i];
		}
		else
		{
			OutPut.ask_price[i] = 0;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			OutPut.ask_volume[i] = InPut.nAskVol[i];
		}
		else
		{
			OutPut.ask_volume[i] = 0;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			OutPut.bid_price[i] = InPut.nBidPrice[i];
		}
		else
		{
			OutPut.bid_price[i] = 0;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			OutPut.bid_volume[i] = InPut.nBidVol[i];
		}
		else
		{
			OutPut.bid_volume[i] = 0;
		}
	}
	return true;
}

bool TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(InPut.nActionDay, InPut.nTime);
	OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(InPut.nActionDay, InPut.nRecvTime));
	OutPut.index = InPut.nIndex;
	OutPut.price = InPut.nPrice;
	OutPut.volume = InPut.nVolume;
	OutPut.ask_order = InPut.nAskOrder;
	OutPut.bid_order = InPut.nBidOrder;
	OutPut.trade_type = make_trade_type((char)InPut.nBSFlag, InPut.chOrderKind, InPut.chFunctionCode);
	return true;
}

bool OrdToOrder(Order& OutPut, SDS20ORDER& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(InPut.nActionDay, InPut.nTime);
	OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(InPut.nActionDay, InPut.nRecvTime));
	OutPut.index = InPut.nOrder;
	OutPut.price = InPut.nPrice;
	OutPut.volume = InPut.nVolume;
	OutPut.order_type = make_order_type(InPut.chOrderKind, InPut.chFunctionCode);
	return true;
}

bool OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	OutPut.ukey = ukey;
	OutPut.trday = atoi(date.c_str());
	OutPut.timeus = GetMsTime(InPut.nActionDay, InPut.nTime);
	OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(InPut.nActionDay, InPut.nRecvTime));
	OutPut.side = InPut.nSide;
	OutPut.price = InPut.nPrice;
	OutPut.orders_num = InPut.nOrders;
	for (int i = 0; i < 50; ++i)
	{
		if (i < InPut.nABItems)
		{
			OutPut.queue[i] = InPut.nABVolume[i];
		}
		else
		{
			OutPut.queue[i] = 0;
		}
	}
	return true;
}

bool InitAndOpenToRead(char* shm_name, int& handle)
{
	int retcode = 0;
	char appname[] = "";
	retcode = shmInit(9901, appname);
	if (retcode != 0)
	{
		LOG(ERROR) << "shmInit error=" << retcode;
		return false;
	}
	retcode = shmOpenForRead(shm_name);
	if (retcode != 0)
	{
		LOG(ERROR) << "shmOpen error=" << retcode << " shm_name : %s" << shm_name;
		return false;
	}
	handle = shmHandle(shm_name);
	int recnum = shmRecnum(handle);
	LOG(INFO) << "recnum=" << recnum << "\t" << shm_name;
	return true;
}

int thread_shm_to_local()
{
	flag_next = true;
	list<std::shared_ptr<std::thread>> m_threadgroup;
	stringstream ss;
	ss << GetTrday();
	date = ss.str();
	auto thread_cfe = [&]() {
		string shm_name = FLAGS_CfePrefix + date;
		shm2file<SDS20FUTURE, Snapshot>(shm_name, date, Cfe_SpiToSnapshot, Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_cfe));

	auto thread_spi = [&]() {
		this_thread::sleep_for(chrono::seconds(3));
		string shm_name = FLAGS_SpiPrefix + date;
		shm2file<SDS20FUTURE, Snapshot>(shm_name, date, Cfe_SpiToSnapshot, Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_spi));

	auto thread_lvt = [&]() {
		this_thread::sleep_for(chrono::seconds(6));
		string shm_name = FLAGS_LvtPrefix + date;
		shm2file<SDS20LEVEL2, Snapshot>(shm_name, date, LvtToSnapshot, Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_lvt));

	auto thread_idx = [&]() {
		this_thread::sleep_for(chrono::seconds(9));
		string shm_name = FLAGS_IdxPrefix + date;
		shm2file<SDS20INDEX, Snapshot>(shm_name, date, IdxToSnapshot, Snapshot2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_idx));

	auto thread_orq = [&]() {
		this_thread::sleep_for(chrono::seconds(12));
		string shm_name = FLAGS_OrqPrefix + date;
		shm2file<SDS20ORDERQUEUE, OrderQueue>(shm_name, date, OrqToOrderqueue, Orderque2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_orq));

	auto thread_ord = [&]() {
		this_thread::sleep_for(chrono::seconds(15));
		string shm_name = FLAGS_OrdPrefix + date;
		shm2file<SDS20ORDER, Order>(shm_name, date, OrdToOrder, Order2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_ord));

	auto thread_trd = [&]() {
		this_thread::sleep_for(chrono::seconds(18));
		string shm_name = FLAGS_TrdPrefix + date;
		shm2file<SDS20TRANSACTION, Transaction>(shm_name, date, TrdToTransaction, Trans2str);
	};
	m_threadgroup.push_back(std::make_shared<std::thread>(thread_trd));

	for (auto &it : m_threadgroup)
	{
		if (it->joinable())
		{
			it->join();
		}
	}
	return 0;
}