#include <cstdio>
#include <iostream>
#include "shm.h"
#include "Trans.h"
#include "SDS20Struct-2017.h"

using namespace std;

bool InitAndOpenToRead(char* shm_name,int& handle)
{
	int retcode = 0;
	char appname[] = "";
	retcode = shmInit(9901, appname);
	if (retcode != 0)
	{
		printf("\n shmInit error=[%d]\n", retcode);
		return false;
	}
	retcode = shmOpenForRead(shm_name);
	if (retcode != 0)
	{
		printf("\n shmOpen error=[%d]\n", retcode);
		return false;
	}
	handle = shmHandle(shm_name);
	int recnum = shmRecnum(handle);
	cout << "recnum=" << recnum << endl;
	return true;
}

int main()
{
	char shm_name[] = "LVT20171024";
	int handle = 0;
	auto ret = InitAndOpenToRead(shm_name, handle);
	if (!ret)
	{
		printf("\n InitAndOpenToRead error\n");
		return -1;
	}
	int i = 0;
	SDS20LEVEL2 temp = { 0 };
	while (true)
	{
		i++;
		memset(&temp, 0, sizeof(temp));
		shmRead(handle, (char*)(&temp), i);
	}
    return 0;
}