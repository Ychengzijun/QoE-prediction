#include"input.h"
#include"fun.h"
#include"User.h"
#include"Pair.h"
#include<iostream>
#include<fstream>
#include<string>
#include<numeric>
using namespace std;

ofstream fcout_train;
ofstream fcout_test;
ofstream fcout_all(".\\output_txt\\all.txt");//所有User的信息
ofstream fcout_starvation;	//(".\\output_txt\\all starvation.txt");//所有经历了starvation的User的信息
ofstream fcout_input(".\\output_txt\\input configure.txt");
ofstream fcout_each_type;

//不同policy下计算throughput
	//计算round_robin policy时的throughput
double throughput_round_robin(double cqi, long number_in_cell)
{
	return cqi / number_in_cell;
}
	//计算max_min policy时的throughput
double throughput_maxmin(vector<long>& v_u)
{
	double throuthput = 0;
	for (int i_t = 0; i_t < number_of_CQI; i_t++)
		throuthput += v_u[i_t] / CQIs[i_t].CQI_value;
	
	return 1 / throuthput;
}

//指数分布发生器
double randomExponential(double lambda)//输出参数为lamuda的指数分布的double值
{  
    double pV = 0.0;  
    while(true)  
    {  
        pV = (double)rand()/(double)RAND_MAX;  
        if (pV != 1)  
        {  
            break;  
        }  
    }  
    pV = (-1.0/lambda)*log(1-pV);  
    return pV;  
}

//Event相关
	//unique_ptr<Event>的比较
bool comp(unique_ptr<Event>& ep1, unique_ptr<Event>& ep2)
{
	return (*ep1) < (*ep2);
}

	/*//这个Event对应的User的位置
unique_ptr<User>& where_this_user(vector<unique_ptr<User>>& v_u, index_aimed i_a)
{
	unique_ptr<User>& p_u = *find_if(v_u.begin(), v_u.end(), i_a);
	return p_u;
}*/

	//如果不是arrival事件，返回true
bool is_not_arrival(unique_ptr<Event>& e_p)
{
	return (e_p->type_value() != Event::arrival);
}

	//删除除了arrival事件的所有事件
void erase_outdated_event(vector<unique_ptr<Event>>& v_e)
{
	auto p_e = remove_if(v_e.begin(), v_e.end(), is_not_arrival);
	v_e.erase(p_e, v_e.end());
}

	//如果是arrival事件，返回true
bool is_arrival(unique_ptr<Event>& e_p)
{
	return (e_p->type_value() == Event::arrival);
}

	//更新arrival事件的时间（因为arrival事件是一个接一个发生的，v_Ep中永远只有一个arrival事件）
void update_arrival_event(vector<unique_ptr<Event>>& v_e, double t_arrival)
{
	v_e[0]->time_update(t_arrival);//v_Ep中只剩下了arrival一个事件
}

	//计算每个活跃User的下个事件
void throw_event(vector<unique_ptr<User>>& v_u, vector<unique_ptr<Event>>& v_e)
{
	for (auto & x : v_u)
		v_e.push_back(unique_ptr<Event>(new Event(x->next_event())));
}

//每个用户在系统中的时长
void each_time_in_cell(vector<unique_ptr<User>>& v_u, double t_last)
{
	for (auto & x : v_u)
		x->time_along(t_last);
}

//记录User arrival时信息
bool comp_Z_info(const Z_info& z1, const Z_info& z2)
{
	return z1.z_index < z2.z_index;
}

	//统计函数
	//原文有的
long number_of_user_incell()
{
	return User::number_of_user_incell_value();
}

vector<long> number_of_user_incell_eachregion()
{
	return User::v_each_region_value();
}

long number_of_user_instarvation(vector<unique_ptr<User>>& v)
{
	long instarvation = 0;
	for (auto & x : v)
	{
		if (x->buffer_state_value() == User::in_starvation)
			instarvation++;
	}

	return instarvation;
}

vector<long> number_of_user_instarvation_eachregion(vector<unique_ptr<User>>& v)
{
	vector<long> v_eachregion(number_of_CQI);
	for (auto & x : v)
	{
		if (x->buffer_state_value() == User::in_starvation)
			v_eachregion[x->CQI_index_value()]++;
	}

	return v_eachregion;
}
	//我加的
long cal_number_to_starvation(vector<unique_ptr<User>>& v)
{
	int n = 0;
	for (auto & x : v)
		if(x->to_starvation())
			n++;
	return n;
}

/*double cal_feature_time_to_stay(vector<unique_ptr<User>>& v)
{
	vector<double> v_time;
	double t_feature;

	for (auto & x : v)
		v_time.push_back(x->expect_time_to_stay());
	sort(v_time.begin(), v_time.end());
	if (v_time.size() % 2 == 1)
		t_feature = v_time[v_time.size() / 2 + 1];
	else
		t_feature = (v_time[v_time.size() / 2] + v_time[v_time.size() / 2 + 1]) / 2;
	
	return t_feature;
}*/

double cal_feature_time_to_stay(vector<unique_ptr<User>>& v)
{
	vector<double> v_time;
	double t_feature;

	for (auto & x : v)
		v_time.push_back(x->expect_time_to_stay());
	t_feature = accumulate(v_time.begin(), v_time.end(), 0.0) / v_time.size();
	
	return t_feature;
}

/*double cal_feature_throughput_in_cell(vector<unique_ptr<User>>& v)
{
	vector<double> v_throughput;
	double t_feature;

	for (auto & x : v)
		v_throughput.push_back(x->throughput_instant_value());
	sort(v_throughput.begin(), v_throughput.end());
	if (v_throughput.size() % 2 == 1)
		t_feature = v_throughput[v_throughput.size() / 2 + 1];
	else
		t_feature = (v_throughput[v_throughput.size() / 2] + v_throughput[v_throughput.size() / 2 + 1]) / 2;
	
	return t_feature;
}*/

double cal_feature_throughput_in_cell(vector<unique_ptr<User>>& v)
{
	vector<double> v_throughput;
	double t_feature;

	for (auto & x : v)
		v_throughput.push_back(x->throughput_instant_value());
	t_feature = accumulate(v_throughput.begin(), v_throughput.end(), 0.0) / v_throughput.size();
	
	return t_feature;
}

vector<long> number_of_user_incell_each_type()
{
	return User::v_each_type_value();
}

Z_info record_z(int switch_button, User& u, vector<unique_ptr<User>>& v)
{
	Z_info z;
	z.z_index = u.index_value();
	z.z_type = u.type_of_streaming_value();
	//原文有的
	z.radio_condition = u.CQI_value();//是CQI还是throughput_instant,CQI吧
	z.video_duration = u.duration_value();
	z.flow_number_in_cell = number_of_user_incell();
	z.flow_number_in_cell_eachregion = number_of_user_incell_eachregion();
	z.flow_number_in_starvation = number_of_user_instarvation(v);
	z.flow_number_in_starvation_eachregion = number_of_user_instarvation_eachregion(v);

	//我加的
	if (switch_button)
	{
		z.num_to_starvation = cal_number_to_starvation(v);
		z.feature_time_to_stay = cal_feature_time_to_stay(v);
		z.feature_throughput_in_cell = cal_feature_throughput_in_cell(v);
		z.flow_number_of_each_type = number_of_user_incell_each_type();
	}

	return z;
}

//记录指定索引的User的z_output
bool comp_Y_info(const Y_info& y1, const Y_info& y2)
{
	return y1.y_index < y2.y_index;
}

Y_info record_y(int switch_button, unique_ptr<User>& p_u)
{
	Y_info r;
	r.y_type = p_u->type_of_streaming_value();
	r.y_index = p_u->index_value();
	//原文的
	r.y_value = p_u->y_output_value();
	//我加的
	if (switch_button)
	{
		r.y_num_starvation = p_u->num_starvation_value();
		r.y_starvation_frequency = p_u->starvation_frequency_value();
		r.y_starvation_frequency_label = p_u->starvation_frequency_label();

		r.y_average_bitrate = p_u->average_bitrate_value();
		r.y_average_bitrate_label = p_u->average_bitrate_label();

		r.y_frequency_bitrate_switch = p_u->frequency_bitrate_switch_value();
		r.y_frequency_bitrate_switch_label = p_u->frequency_bitrate_switch_label();
	}
	//其他
	r.y_time_along = p_u->time_in_cell_value();

	return r;
}

//User相关
	//更新其他每个活跃User的buffer信息
void update_other_buffer_infomation(vector<unique_ptr<User>>& v_Up, long index_last, double t_last_e)
{
	for (auto & x : v_Up)
	{
		if (x->index_value() != index_last)
			x->buffer_infomation_update(t_last_e);
	}
}

	//更新每个活跃User的throughput_instant
void update_throughput(vector<unique_ptr<User>>& v) //更新所有User的throughput_instant
{
	double same_throughput = 0;
	switch(current_policy)
	{
	case round_robin:
		for (auto & x : v)
			x->throughput_instant_update();
		break;
	case max_min:
		if (v.size())
		{
			v[0]->throughput_instant_update();
			same_throughput = v[0]->throughput_instant_value();
			for (auto & x : v)
				x->throughput_set(same_throughput);
		}
		break;
	case max_ci:

		break;
	}

}

//输出统计信息和数据集
	//计数
long count_starvation_of_vp(vector<Pair>& v_c)
{
	long number = 0;
	for (auto x: v_c)
		if (x.y_pair_value().y_value == 1)
			number++;
	return number;
}
	//数据集处理
vector<Pair>& delete_unstable_in_begin_end(int switch_button, vector<Pair>& v_d)
{
	if (switch_button)
	{
		v_d.erase(v_d.begin(), v_d.begin() + int(v_d.size() * rate_unstable_in_begin_end));
		v_d.erase(v_d.end() - int(v_d.size() * rate_unstable_in_begin_end), v_d.end());
	}

	return v_d;
}

void output_train_test(vector<Pair>& v_p_d, const char* s_train, const char* s_test)
{
	fcout_train.open(s_train); 
	fcout_test.open(s_test);
	int i_s = 0;

	random_shuffle(v_p_d.begin(), v_p_d.end());
	for (auto x : v_p_d)
	{
		if (i_s < rate_for_training * v_p_d.size())//随机选择80%，starvation较少时可能会有特殊情况
			fcout_train << x;
		else
			fcout_test << x;
		i_s++;
	}
	fcout_train.close();
	fcout_test.close();
	fcout_train.clear();
	fcout_test.clear();
}

vector<vector<Pair>> classify_into_show_each_type(vector<Pair>& v_p_arg)
{
	vector<vector<Pair>> v_v_each_type(4);//分成四种类型的vector<Pair>
	for (auto x : v_p_arg)
		v_v_each_type[x.z_pair_value().z_type].push_back(x);

	return v_v_each_type;
}
	//平均时间
double average_time_incell(vector<Pair>& v_p_arg)
{
	double time_in_cell_total = 0;
	for (auto x : v_p_arg)
		time_in_cell_total += x.y_pair_value().y_time_along;

	return time_in_cell_total / v_p_arg.size();
}
	//输出
void output_whole(vector<Pair>& v_p_arg)
{
	fcout_all << "R,T,X,X1,X2,X3,X4,X5,S,S1,S2,S3,S4,S5,starvation,num_starvation,RF_value,RF,MB_value,MB,SF_value,SF" << endl;
	for (auto x : v_p_arg)
		fcout_all << x;
}

void output_starvation(vector<Pair>& v_p_arg)
{
	vector<Pair> v_starvation;//所有starvation的
	for (auto x : v_p_arg)
	{
		if (x.y_pair_value().y_value == 1)
		{
			v_starvation.push_back(x);
			fcout_starvation << x;
		}
	}
}

void show_statistics(ostream& os, long total_user, vector<long>& number_each_type_arg, double average_time_arg, 
					 long total_starvation, vector<long>& number_each_type_starvation_arg)
{
	os << "number of User: " << total_user << endl;//总用户数
	os << "number of each type: ";//每种streaming，用户数
	for (int i_s = 0; i_s < 4; i_s++)
		os << number_each_type_arg[i_s] << "\t";
	cout << endl;

	os << "average time in cell: " << average_time_arg << endl;//平均in_cell时间

	os << "number_of_starvation: " << total_starvation << endl;//总卡顿用户数
	os << "number of each type: ";//每种streaming，卡顿用户数
	for (int i_s = 0; i_s < 4; i_s++)
		os << number_each_type_starvation_arg[i_s] << "\t";
	cout << endl;
}

void show_output(vector<Pair>& v_p_arg)
{
	double average_time = average_time_incell(v_p_arg);
	auto v_each_type = classify_into_show_each_type(v_p_arg);//为什么eclips下不能用引用
	vector<long> number_each_type(4);
	for (int i_s = 0; i_s < 4; i_s++)
		number_each_type[i_s] = v_each_type[i_s].size();

	vector<long> number_starvation_each_type(4);
	for (int i_s = 0; i_s < 4; i_s++)
		number_starvation_each_type[i_s] = count_starvation_of_vp(v_each_type[i_s]);
	long number_starvation_total = accumulate(number_starvation_each_type.begin(), number_starvation_each_type.end(), 0);
	
	//输出all_User
	output_whole(v_p_arg);
	
	//输出all_starvation
	output_starvation(v_p_arg);

	//输出统计信息
	//控制台.txt
	show_statistics(cout, v_p_arg.size(), number_each_type, average_time, number_starvation_total, number_starvation_each_type);
	//all.txt
	show_statistics(fcout_all, v_p_arg.size(), number_each_type, average_time, number_starvation_total, number_starvation_each_type);
	//all starvation.txt
	show_statistics(fcout_starvation, v_p_arg.size(), number_each_type, average_time, number_starvation_total, number_starvation_each_type);

	//输出数据集
	//8个starvation dataset
	output_train_test(v_each_type[0], ".\\output_txt\\static_adaptive_training.txt", ".\\output_txt\\static_adaptive_test.txt");
	output_train_test(v_each_type[1], ".\\output_txt\\mobile_adaptive_training.txt", ".\\output_txt\\mobile_adaptive_test.txt");
	output_train_test(v_each_type[2], ".\\output_txt\\static_fixed_training.txt", ".\\output_txt\\static_fixed_test.txt");
	output_train_test(v_each_type[3], ".\\output_txt\\mobile_fixed_training.txt", ".\\output_txt\\mobile_fixed_test.txt");
	//2个
	output_train_test(v_p_arg, ".\\output_txt\\all_training.txt", ".\\output_txt\\all_test.txt");
}

//输出总的数据集
void output_to_specified_file(vector<Pair>& v_p_d, const char* type_name)	//一个输出文件的函数应该怎么样设计
{
	fcout_each_type.open(type_name);
	fcout_each_type << "R,T,X,X1,X2,X3,X4,X5,S,S1,S2,S3,S4,S5,starvation,num_starvation,RF_value,RF,MB_value,MB,SF_value,SF" << endl;
	for (auto x : v_p_d)
		fcout_each_type << x;
	fcout_each_type.close();
	fcout_each_type.clear();
}

void output_four_type(vector<Pair>& v_p_arg)
{
	auto v_each_type = classify_into_show_each_type(v_p_arg);
	output_whole(v_p_arg);
	output_to_specified_file(v_each_type[0], ".\\output_txt\\static_adaptive.txt");
	output_to_specified_file(v_each_type[1], ".\\output_txt\\mobile_adaptive.txt");
	output_to_specified_file(v_each_type[2], ".\\output_txt\\static_fixed.txt");
	output_to_specified_file(v_each_type[3], ".\\output_txt\\mobile_fixed.txt");
}
//显示输入值设定
void show_input()
{
	fcout_input << "Input Value: \n\n";
	
	fcout_input << "number of User: " << l << endl;
	fcout_input << "Set of v: ";
	int number_of_v_adaptive = sizeof(set_bitrate_adaptive) / sizeof(set_bitrate_adaptive[0]);
	for (int i_fcout = 0; i_fcout < number_of_v_adaptive; i_fcout++)
		fcout_input << "v" << i_fcout << ": " << set_bitrate_adaptive[i_fcout] << "\t";
	fcout_input << "\nDefault_bitrate_fixed: " << default_bitrate_fixed << endl;
	fcout_input << "Mean of duration: " << T << endl;
	fcout_input << "Arrival_load_current: " << arrival_load_current << endl;
}

