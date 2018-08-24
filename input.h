#ifndef INPUT_H_
#define INPUT_H_

#include<vector>
#include<ctime>
#include<cstdlib>
#include<cmath>
#include<algorithm>

//CQI；可以把使用CQI的函数都更一般化
const int number_of_CQI = 5;//CQI个数
struct set_of_CQI
{
	double CQI_value;
	double CQI_pro;
};
const set_of_CQI CQIs[number_of_CQI] = {
	{49.24,0.111},
	{31.89,0.294},
	{18.84,0.375},
	{9.22,0.192},
	{2.975,0.028}
	};//set_of_throughput，应当是可变的

const double B = 1;
const int h = 2;//chunk_duration
const int l = 100;//仿真总用户数
const double set_bitrate_adaptive[] = {1, 1.5, 2, 2.5};//可选下载速率集合
const double default_bitrate_fixed = set_bitrate_adaptive[1];
const double first_bitrate_adaptive = set_bitrate_adaptive[0];
const double second_bitrate_adaptive = set_bitrate_adaptive[1];
const double third_bitrate_adaptive = set_bitrate_adaptive[2];
const double fourth_bitrate_adaptive = set_bitrate_adaptive[3];
const double T = 40;//duration均值
const double arrival_rate_max = 0.37;//最大arrival_rate
const double arrival_load_current = 0.865;//本次仿真用的rate
const double load_normalized[] = {0.108, 0.216, 0.324, 0.432, 0.54, 0.648, 0.756, 0.865};//8次实验load归一化的比例
const double mobility_rate = 1;
const long default_number_in_cell = 0;

enum schedule_policy{round_robin, max_min, max_ci};
const schedule_policy current_policy = round_robin;

//数据处理开关和参数
const int switch_delete_unstable_data = 0;
const double rate_for_training = 0.8;
const double rate_unstable_in_begin_end = 0.01;

//输出流开关
const int switch_show_buffer_information = 0;
const int switch_show_process_information = 1;

//输入参数record_z开关
const int switch_record_my_features = 0;

//输入参数record_y开关
const int switch_record_my_targets = 1;

//label阈值
const double treshold_starvation_frequency = 0.1;
const double treshold_average_bitrate_first = 1.5;
const double treshold_average_bitrate_second = 2.0;
const double treshold_bitrate_switch_static = 0.05;
const double treshold_bitrate_switch_mobile = 0.2;

#endif
