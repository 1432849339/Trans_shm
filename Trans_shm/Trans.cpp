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

void LvtToSnapshot(Snapshot& OutPut, map<int64_t, OrderQueue>& AskQue, map<int64_t, OrderQueue>& BidQue, SDS20LEVEL2& Input)
{
	string windcode = string(Input.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = Input.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, Input.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, Input.nRecvTime);
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
	//OutPut.ask_orders_num=
}

void IdxToSnapshot(Snapshot& OutPut, SDS20INDEX& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = InPut.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, InPut.nRecvTime);
	OutPut.status = 0;
	OutPut.pre_close = InPut.nPreCloseIndex;
	OutPut.high = InPut.nHighIndex;
	OutPut.low = InPut.nLowIndex;
	OutPut.open = InPut.nOpenIndex;
	OutPut.last = InPut.nLastIndex;
	OutPut.match_num = 0;
	OutPut.volume = InPut.iTotalVolume;
	OutPut.turnover = InPut.iTurnover;
}

void Cfe_SpiToSnapshot(Snapshot& OutPut, SDS20FUTURE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = InPut.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, InPut.nRecvTime);
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
}

void TrdToTransaction(Transaction& OutPut, SDS20TRANSACTION& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = InPut.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, InPut.nRecvTime);
	OutPut.index = InPut.nIndex;
	OutPut.price = InPut.nPrice;
	OutPut.volume = InPut.nVolume;
	OutPut.ask_order = InPut.nAskOrder;
	OutPut.bid_order = InPut.nBidOrder;
	OutPut.trade_type = make_trade_type((char)InPut.nBSFlag, InPut.chOrderKind, InPut.chFunctionCode);
}

void OrdToOrder(Order& OutPut, SDS20ORDER& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = InPut.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, InPut.nRecvTime);
	OutPut.index = InPut.nOrder;
	OutPut.price = InPut.nPrice;
	OutPut.volume = InPut.nVolume;
	OutPut.order_type = make_order_type(InPut.chOrderKind, InPut.chFunctionCode);
}

void OrqToOrderqueue(OrderQueue& OutPut, SDS20ORDERQUEUE& InPut)
{
	string windcode = string(InPut.szWindCode);
	int64_t ukey = GetUkey(windcode.substr(windcode.rfind('.') + 1), windcode.substr(0, windcode.rfind('.')));
	OutPut.ukey = ukey;
	OutPut.trday = InPut.nActionDay;
	OutPut.timeus = GetMsTime(OutPut.trday, InPut.nTime);
	OutPut.recvus = GetMsTime(OutPut.trday, InPut.nRecvTime);
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
}