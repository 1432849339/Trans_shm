#ifndef __PATHHELPER_H__
#define __PATHHELPER_H__
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

using namespace std;
using namespace boost::filesystem;
//深度目录迭代器
typedef recursive_directory_iterator	rd_iterator;
//只遍历本层目录
bool TraverseDireatorOnce(path& dir, queue<path>& files_vec);
//深度遍历目录
bool TraverseDireatorDeepin(path& dir, queue<path>& files_vec);
//创建文件,失败返回false
bool CreateDir(string  pathName);
//增加错误信息
bool CreateDir(string pathName, string& error_code);
//删除文件
bool DeleteDir(path& dir);
//删除文件,并增加错误信息
bool DeleteDir(path& dir,string& error_code);
//将整行数字转换为字符串
string itostr(const int64_t num);
//有年月日时分秒合成时间戳,时间戳单位为微妙(如:20170904 93000000)
int64_t GetMsTime(int64_t ymd, int64_t hmsu);

string DateSub(string& ymd_str, int num);

#endif





