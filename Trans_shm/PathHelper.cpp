#include "PathHelper.h"


bool TraverseDireatorOnce(path& dir, queue<path>& files_vec)
{
	//只遍历本层目录
	path dir2(dir);
	try
	{
		directory_iterator end1;
		for (directory_iterator pos(dir2); pos != end1; pos++)
		{
			if (is_directory(*pos))
			{
				files_vec.emplace(*pos);
			}
		}
		return true;
	}
	catch (boost::filesystem::filesystem_error& e)
	{
		cerr << e.what() << endl;
		return false;
	}
}

bool TraverseDireatorDeepin(path& dir, queue<path>& files_vec)
{
	path dir2(dir);
	//深度遍历
	try
	{
		rd_iterator end2;
		for (rd_iterator pos(dir2); pos != end2; pos++)
		{
			//如果深度大于4层，则不再继续深入
			if (is_directory(*pos) && pos.level() > 10)
			{
				pos.no_push();
			}
			//如果该目录下有nofind.txt文件，则跳出该目录
			if (*pos == "nofind.txt")
			{
				pos.pop();
			}
			if (is_regular_file(*pos))
			{
				files_vec.emplace(*pos);
			}
		}
		return true;
	}
	catch (boost::filesystem::filesystem_error& e)
	{
		cout << e.what() << endl;
		return false;
	}
}

bool CreateDir(string pathName)
{
	path  dir(pathName);
	try {
		
		if (!boost::filesystem::exists(dir))
		{
			if (!boost::filesystem::create_directories(dir)) {
				return false;
			}
		}
		return true;
	}
	catch (boost::filesystem::filesystem_error&e) {
		cout << e.what() << endl;
		return false;
	}
}

bool CreateDir(string pathName,string& error_code)
{
	path  dir(pathName);
	try {

		if (!boost::filesystem::exists(dir))
		{
			if (!boost::filesystem::create_directories(dir)) {
				return false;
			}
		}
		return true;
	}
	catch (boost::filesystem::filesystem_error&e) {
		error_code = e.what();
		return false;
	}
}

bool DeleteDir(path & dir)
{
	try
	{
		if (boost::filesystem::exists(dir))
		{
			boost::filesystem::remove_all(dir);
		}
	}
	catch (filesystem_error& e)
	{
		cout << e.what() << endl;
		return false;
	}
	return true;
}

bool DeleteDir(path& dir, string& error_code)
{
	try
	{
		if (boost::filesystem::exists(dir))
		{
			boost::filesystem::remove_all(dir);
		}
	}
	catch (filesystem_error& e)
	{
		error_code = e.what();
		return false;
	}
	return true;
}

string itostr(const int64_t num)
{
	char num_str[32];
	sprintf(num_str, "%ld", num);
	return string(num_str);
}

int64_t GetMsTime(int64_t ymd, int64_t hmsu)
{
	struct tm timeinfo = { 0 };
	time_t second;
	int64_t  usecond;
	timeinfo.tm_year = int(ymd / 10000 - 1900);
	timeinfo.tm_mon = int((ymd % 10000) / 100 - 1);
	timeinfo.tm_mday = int(ymd % 100);
	second = mktime(&timeinfo);
	//80000000

	int hou = (int)(hmsu / 10000000);
	int min = (int)((hmsu % 10000000) / 100000);
	int sed = (int)((hmsu % 100000) / 1000);
	int used = (int)(hmsu % 1000);

	usecond = second + hou * 3600 + min * 60 + sed;
	usecond *= 1000000;
	usecond += used * 1000;
	return usecond;
}

string DateSub(string& ymd_str, int num)
{
	int ymd = atoi(ymd_str.c_str());
	time_t tt;
	struct tm timeinfo = { 0 };
	timeinfo.tm_year = int(ymd / 10000 - 1900);
	timeinfo.tm_mon = int((ymd % 10000) / 100 - 1);
	timeinfo.tm_mday = int(ymd % 100);
	tt = mktime(&timeinfo);
	//减去num天
	tt -= 60 * 60 * 24 * num;
	tm* pstm = localtime(&tt);
	int year = pstm->tm_year + 1900;
	int mon = pstm->tm_mon + 1;
	int day = pstm->tm_mday;
	ymd = year * 10000 + mon * 100 + day;
	return itostr(ymd);
}
