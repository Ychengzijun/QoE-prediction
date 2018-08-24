#ifndef FUN_H_
#define FUN_H_

#include"Event.h"
#include"User.h"
#include<memory>

class index_aimed
{
private:
	long aim;
public:
	index_aimed(long a) : aim(a) {}
	bool operator()(unique_ptr<User>& e) { return e->index_value() == aim; }
};

//不同policy下计算throughput
	//计算round_robin policy时的throughput
double throughput_round_robin(double cqi, long number_in_cell);
	//计算max_min policy时的throughput
double throughput_maxmin(vector<long>& v_u);

//指数分布发生器
double randomExponential(double lambda);//返回参数为1 / lambda的指数分布的double值//duration，arrival_time

//Event相关
	//unique_ptr<Event>的比较
bool comp(unique_ptr<Event>& ep1, unique_ptr<Event>& ep2);

	//这个Event对应的User的位置
unique_ptr<User>& where_this_user(vector<unique_ptr<User>>& v_u, index_aimed i_a);

	//如果不是arrival事件，返回true
bool is_not_arrival(unique_ptr<Event>& e_p);

	//删除除了arrival事件的所有事件
void erase_outdated_event(vector<unique_ptr<Event>>& v_e);

	//更新arrival事件的时间（因为arrival事件是一个接一个发生的，v_Ep中永远只有一个arrival事件）
void update_arrival_event(vector<unique_ptr<Event>>& v_e, double t_arrival);

	//计算每个活跃User的下个事件
void throw_event(vector<unique_ptr<User>>& v_u, vector<unique_ptr<Event>>& v_e);

//每个用户在系统中的时长
void each_time_in_cell(vector<unique_ptr<User>>& v_u, double t_last);

//记录User arrival时信息
	//信息格式
struct Z_info
{
	long z_index;
	int z_type;
	double radio_condition;
	int video_duration;
	long flow_number_in_cell;
	vector<long> flow_number_in_cell_eachregion;
	long flow_number_in_starvation;
	vector<long> flow_number_in_starvation_eachregion;

	long num_to_starvation;//变化率小于0的用户数 //小于0的比例呢，平均欠缺率
	double feature_time_to_stay;//当前系统中用户预期停留时长的feature，当前系统中用户平均完成比例
	double feature_throughput_in_cell;//系统平均吞吐量
	vector<long> flow_number_of_each_type;
	
};
bool comp_Z_info(const Z_info& z1, const Z_info& z2);
	//统计函数
	//原文有的
vector<long> number_of_user_incell_eachregion();//接受v_Up为参数，计算number_of_user_incell_eachregion，返回vector
long number_of_user_instarvation(vector<unique_ptr<User>>& v);//接受v_Up为参数，计算number_of_user_instarvation，返回
vector<long> number_of_user_instarvation_eachregion(vector<unique_ptr<User>>& v);//接受v_Up为参数，计算number_of_user_instarvation_eachregion，返回vector
	//我加的
long cal_number_to_starvation(vector<unique_ptr<User>>& v);//当前系统中，有卡顿趋势的用户数
double cal_feature_time_to_stay(vector<unique_ptr<User>>& v);//当前系统中用户，剩余时间的一个指标
double cal_feature_throughput_in_cell(vector<unique_ptr<User>>& v);//当前系统吞吐量的feature
vector<long> number_of_user_incell_each_type();//当前系统中各类的用户数

Z_info record_z(int switch_button, User& u, vector<unique_ptr<User>>& v);//User arrival时记录User的信息

//记录指定索引的User的z_output
struct Y_info
{
	long y_index;
	int y_type;
	//原文的
	int y_value;
	//我加的
	int y_num_starvation;
	double y_starvation_frequency;
	string y_starvation_frequency_label;
	double y_average_bitrate;
	string y_average_bitrate_label;
	double y_frequency_bitrate_switch;
	string y_frequency_bitrate_switch_label;
	
	//其他
	double y_time_along;
};
bool comp_Y_info(const Y_info& y1, const Y_info& y2);
Y_info record_y(int switch_button, unique_ptr<User>& p_u);

//User相关
	//更新其他每个活跃User的buffer信息
void update_other_buffer_infomation(vector<unique_ptr<User>>& v_Up, long index_last, double t_last_e);

	//更新每个活跃User的throughput_instant
void update_throughput(vector<unique_ptr<User>>& v);

//输出统计信息和数据集
class Pair;
long count_starvation_of_vp(vector<Pair>& v_c);//统计此v中发生starvation的个数
vector<Pair>& delete_unstable_in_begin_end(int switch_button, vector<Pair>& v_d);//依制定比例删除开始和结束时的数据
void output_train_test(vector<Pair>& v_p_d, const char* s_train, const char* s_test);//给定vector，输出按指定比例随机分组的指定名称的训练集和测试集，
double average_time_incell(vector<Pair>& v_p_arg);//统计在系统中的平均时间
void output_whole(vector<Pair>& v_p_arg);//输出all_User
void output_starvation(vector<Pair>& v_p_arg);//输出all_starvation
vector<vector<Pair>> classify_into_show_each_type(vector<Pair>& v_p_arg);//分类，返回
void show_statistics(ostream& os, long total_user, vector<long>& number_each_type_arg, double average_time_arg, 
					 long total_starvation, vector<long>& number_each_type_starvation_arg);//输出统计信息
void show_output(vector<Pair>& v_p_arg);//输出统计信息和数据集

//输出总的数据集
	//输出到指定文件
void output_to_specified_file(vector<Pair>& v_p_d, const char* type_name);
	//将总文件分成四种分别输出
void output_four_type(vector<Pair>& v_p_arg);
//显示输入值设定
void show_input();

#endif
