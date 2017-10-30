#include "Trans.h"




int64_t GetMsTime(int64_t ymd, int64_t hmsu)
{
	struct tm timeinfo = { 0 };
	time_t second;
	int64_t  usecond;
	timeinfo.tm_year = ymd / 10000 - 1900;
	timeinfo.tm_mon = (ymd % 10000) / 100 - 1;
	timeinfo.tm_mday = ymd % 100;
	second = mktime(&timeinfo);
	//80000000
	int hou = hmsu / 10000000;
	int min = (hmsu % 10000000) / 100000;
	int sed = (hmsu % 100000) / 1000;
	int used = hmsu % 1000;

	usecond = second + hou * 3600 + min * 60 + sed;
	usecond *= 1000000;
	usecond += used * 1000;
	return usecond;
}

time_t GetTr_time()
{
	time_t tt;
	struct tm* pstm = nullptr;
	tt = time(nullptr);
	pstm = localtime(&tt);
	if (pstm->tm_wday == 6)//星期6
	{
		tt += 2 * 24 * 60 * 60;
	}
	else if (pstm->tm_wday == 0)//星期天
	{
		tt += 1 * 24 * 60 * 60;
	}
	else if ((pstm->tm_wday == 5) &&
		((pstm->tm_hour == SPLIT_HROUS && pstm->tm_min >= SPLIT_MINN) || (pstm->tm_hour > SPLIT_HROUS))
		)
	{
		tt += 3 * 24 * 60 * 60;
	}
	else if ((pstm->tm_wday >= 1 && pstm->tm_wday <= 4) &&
		((pstm->tm_hour == SPLIT_HROUS && pstm->tm_min >= SPLIT_MINN) || (pstm->tm_hour > SPLIT_HROUS))
		)//星期1到星期4
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
	if (pstm->tm_wday == 6)//星期6
	{
		tt += 2 * 24 * 60 * 60;
	}
	else if(pstm->tm_wday == 0)//星期天
	{
		tt += 1 * 24 * 60 * 60;
	}
	else if((pstm->tm_wday == 5) && 
		  ((pstm->tm_hour == SPLIT_HROUS && pstm->tm_min >= SPLIT_MINN) || (pstm->tm_hour > SPLIT_HROUS))
		)
	{
		tt += 3 * 24 * 60 * 60;
	}
	else if((pstm->tm_wday >= 1 && pstm->tm_wday <= 4) && 
		((pstm->tm_hour == SPLIT_HROUS && pstm->tm_min >= SPLIT_MINN) || (pstm->tm_hour > SPLIT_HROUS))
		)//星期1到星期4
	{
		tt += 1 * 24 * 60 * 60;
	}
	pstm = localtime(&tt);
	int year = pstm->tm_year + 1900;
	int mon = pstm->tm_mon + 1;
	int day = pstm->tm_mday;
	return year * 10000 + mon * 100 + day;
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
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = Input.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, Input.nTime);
		OutPut.recvus = (Input.nRecvTime < Input.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, Input.nRecvTime));
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
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = InPut.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
		OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, InPut.nRecvTime));
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
}

bool Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = InPut.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
		OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, InPut.nRecvTime));
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
}

bool TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = InPut.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
		OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, InPut.nRecvTime));
		OutPut.index = InPut.nIndex;
		OutPut.price = InPut.nPrice;
		OutPut.volume = InPut.nVolume;
		OutPut.ask_order = InPut.nAskOrder;
		OutPut.bid_order = InPut.nBidOrder;
		OutPut.trade_type = make_trade_type((char)InPut.nBSFlag, InPut.chOrderKind, InPut.chFunctionCode);
		return true;
	}
}

bool OrdToOrder(Order& OutPut, SDS20ORDER& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = InPut.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
		OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, InPut.nRecvTime));
		OutPut.index = InPut.nOrder;
		OutPut.price = InPut.nPrice;
		OutPut.volume = InPut.nVolume;
		OutPut.order_type = make_order_type(InPut.chOrderKind, InPut.chFunctionCode);
		return true;
	}
}

bool OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	if (0 == ukey)
	{
		return false;
	}
	else
	{
		OutPut.ukey = ukey;
		OutPut.trday = InPut.nActionDay;
		OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
		OutPut.recvus = (InPut.nRecvTime < InPut.nTime ? OutPut.timeus : GetMsTime(OutPut.trday, InPut.nRecvTime));
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
}