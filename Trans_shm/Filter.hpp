#ifndef __FILTER_H__
#define __FILTER_H__

#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <type_traits>
#include <typeinfo>
#include <iterator>

class DateNum
{
public:
	DateNum() :m_hourMinu(0)
	{
	}

	DateNum(int HourMinu) :m_hourMinu(HourMinu) 
	{
	}

	DateNum(const DateNum& date_num) 
	{
		m_hourMinu = date_num.m_hourMinu;
	}

	DateNum& operator= (const DateNum& date_num)
	{
		this->m_hourMinu = date_num.m_hourMinu;
		return *this;
	}

	DateNum& operator= (DateNum&& date_num)
	{
		this->m_hourMinu = std::move(date_num.m_hourMinu);
		return *this;
	}

	bool operator<(const DateNum& date_num) const
	{
		return this->m_hourMinu < date_num.m_hourMinu;
	}
	//前置++
	DateNum & operator++()
	{
		int minu = m_hourMinu % 100;
		int hour = m_hourMinu / 100;
		if (minu >= 59)
		{
			minu = 0;
			hour++;
			this->m_hourMinu = hour * 100 + minu;
		}
		else 
		{
			this->m_hourMinu++;
		}
		return *this;
	}
	//后置++
	DateNum operator++(int)
	{
		DateNum temp = *this;
		int minu = m_hourMinu % 100;
		int hour = m_hourMinu / 100;
		if (minu >= 59)
		{
			minu = 0;
			hour++;
			this->m_hourMinu = hour * 100 + minu;
		}
		else
		{
			this->m_hourMinu++;
		}
		this->m_hourMinu++;
		return temp;
	}
private:
	int m_hourMinu;
};

class MarketFilter
{
public:
	//增加规则参数,以容器的方式,支持vector list set
	template<class Container>
	void AddNumToRuler(Container&& contain)
	{
		for (auto &it:contain)
		{
			m_rulers.emplace(it);
		}
	}
	//增加一个规则参数,int
	void AddNumToRuler(int value)
	{
		m_rulers.emplace(value);
	}
	//删除一个规则内的参数
	void DeleteOneRuler(int value)
	{
		m_rulers.erase(value);
	}
	//包含在规则内
	bool In_Ruler(int num) 
	{
		return m_rulers.find(num) != m_rulers.end();
	}
private:
	std::set<DateNum>  m_rulers;
};

#endif //!__MARKETFILTER_H__