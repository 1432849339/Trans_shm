#include "Trans.h"

using namespace std;
string date;

DEFINE_string(VERSION_STR, "0.0.1", "版本信息");
DEFINE_string(log_path, "./log", "日志目录");
DEFINE_bool(conn_way, true, "是否用文件的方式登陆");

int main(int argc,char *argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::SetVersionString(FLAGS_VERSION_STR);
	string usage = string(FLAGS_VERSION_STR) + "\nUsage: exe_obj --flagfile config.txt";
	google::SetUsageMessage(usage);
	google::ParseCommandLineFlags(&argc, &argv, false);

	FLAGS_log_dir = FLAGS_log_path;
	FLAGS_stop_logging_if_full_disk = true;
	FLAGS_logbufsecs = 0;
	FLAGS_max_log_size = 100;

	if (!CreateDir(FLAGS_log_dir.c_str()))
	{
		printf("Create log dir faild! log_dir=%s\n", FLAGS_log_dir.c_str());
		return -2;
	}

	set<string> market_set;
	if (!FLAGS_conn_way)
	{
		if (!Init_ukdb09(market_set, false))
		{
			LOG(ERROR) << "init database ukdb09 error";
			return -1;
		}
	}
	else
	{
		if (!Init_ukdb09(market_set, true))
		{
			LOG(ERROR) << "init database ukdb09 error";
			return -1;
		}
	}
	
	vector<shared_ptr<thread>>  thdsGroup;
	thdsGroup.emplace_back(make_shared<thread>(thread_shm_to_local));
	thdsGroup.emplace_back(make_shared<thread>(is_next_day));
	
	LOG(INFO) << "开始实时数据生成  date = " << date;
	for (auto &ths:thdsGroup)
	{
		LOG(INFO) << "线程id:" << ths->get_id() << " 启动...";
		ths->join();
	}
    return 0;
}


