#include "CreatePath.h"


const string path = "/UKData/TDF/fqy/Request.csv/";
extern string date;


string itostr(int64_t num)
{
	char num_str[32];
	sprintf(num_str, "%ld", num);
	return string(num_str);
}

bool CreatePath(string Path)
{
	if (access(Path.c_str(), 0) == -1)
	{
		if (mkdir(Path.c_str(), 0777))
		{
			cout << "make errorly" << __LINE__ << endl;
			return false;
		}
	}
	return true;
}

string Create_sub_path(int64_t ukey, string BasePath, string date)
{
	string::size_type pos = 0;
	while ((pos = BasePath.find('/', pos)) != string::npos)
	{
		string SubStr = BasePath.substr(0, pos);
		if (access((path + SubStr).c_str(), 0) == -1)
		{
			if (mkdir((path + SubStr).c_str(), 0777))
			{
				cout << "make errorly" << __LINE__ << endl;
				return string();
			}
		}
		pos++;
	}
	//ukey
	BasePath = path + BasePath + itostr(ukey) + "/";
	if (access(BasePath.c_str(), 0) == -1)
	{
		if (mkdir((BasePath).c_str(), 0777))
		{
			cout << "make errorly" << __LINE__ << endl;
			return string();
		}
	}
	//year
	BasePath += date.substr(0, 4) + "/";
	if (access(BasePath.c_str(), 0) == -1)
	{
		if (mkdir((BasePath).c_str(), 0777))
		{
			cout << "make errorly" << __LINE__ << endl;
			return string();
		}
	}
	//ymd
	BasePath += date + "/";
	if (access(BasePath.c_str(), 0) == -1)
	{
		if (mkdir((BasePath).c_str(), 0777))
		{
			cout << "make errorly" << __LINE__ << endl;
			return string();
		}
	}
	return BasePath;
}