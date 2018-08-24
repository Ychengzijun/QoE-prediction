#ifndef User_H_
#define User_H_

#include"input.h"
#include"Event.h"
#include<string>
using namespace std;

class User
{
public:
	friend class Event;//这是最佳的处理吗
	enum {active, unactive};//state
	enum {in_prefetch, in_play, in_starvation};//buffer_state
	enum {static_adaptive, mobile_adaptive, static_fixed, mobile_fixed};//type_of_streaming	
	enum type_process_hint{hint_arrival, hint_departurel, hint_chunk, hint_play, hint_starvation, hint_mobility};//show_process_information

	//用户数相关
	static long number_of_user_incell_value() { return number_of_user_incell; }
	static vector<long>& v_each_type_value() { return v_each_type; }
	static vector<long>& v_each_region_value() { return v_each_region; }

	//User相关
	User(long index_ctor, double t_c);
	User(long index_ctor, int CQI_index_ctor, int type_ctor, int duration_ctor, double t_c);
	long index_value() { return index; }
	int state_value() { return state; }
	void into_unactive();
	int type_of_streaming_value() { return type_of_streaming; }
	int duration_value() { return duration; }

	//CQI相关
	double CQI_value() { return CQI; }
	int CQI_index_value() {return CQI_index; }
	double CQI_ini();//初始CQI
	void CQI_next();
	
	//buffer相关
	int buffer_state_value() { return buffer_state; }
	double throughput_instant_ini();
	void throughput_instant_update();//基于|X|重新计算瞬时吞吐量
	double throughput_instant_value() { return throughput_instant; }
	void throughput_set(double throughput_arg) { throughput_instant = throughput_arg; }
	double bitrate_initial();
	void bitrate_downloading_current_chunk_update();//根据throughput_instant选择一个v
	void downloading_new_chunk();//开始新的chunk
	void play_about_information(double t_play);
	void starvation_left_update(double t_s_l_u);
	void next_mobility_update(double t_n_m_u);
	void buffer_infomation_update(double t_buffer_infomation_update);

	bool to_starvation() { return throughput_instant < bitrate_playing_current_chunk; }//buffer变化率是否小于0
	double expect_time_to_stay();

	//事件发生器
	double time_until_starvation();
	double time_until_chunk();
	double time_until_play();
	double time_until_mobility();
	class Event next_event();//返回下次事件的时间间隔

	//事件处理器
	void get_event_departure(double t_get_departure);
	void get_event_chunk(double t_get_chunk);
	void get_event_starvation(double t_starvation);
	void get_event_play(double t_play);
	void get_event_mobility(double t_mobility);

	//预测目标
	int y_output_value() { return y_output; }
	int num_starvation_value() { return num_starvation; }
	double starvation_frequency_value() { return double(num_starvation) / duration; }
	string starvation_frequency_label();
	double average_bitrate_value();//历史平均比特率
	string average_bitrate_label();
	int number_bitrate_switch_value() { return number_bitrate_switch; }
	double frequency_bitrate_switch_value() { return double(number_bitrate_switch) / duration; }
	string frequency_bitrate_switch_label();
	void time_along(double t_last) { time_in_cell += t_last; }
	double time_in_cell_value() { return time_in_cell; }

	//显示用
	void show_buffer_information(int switch_button);
	void show_process_information(type_process_hint t_p_h, double t_hint, int switch_button);
	static void show_number_each_type();
private:
	//用户数相关
	static long number_of_user_incell;
	static vector<long> v_each_type;
	static vector<long> v_each_region;

	//User相关
	long index;
	int state;
	int type_of_streaming;//如果分成两组，怎么保证四分之一？是否应该用位运算
	double next_mobility;//距离下次移动的事件间隔
	int duration;//初始化为均值为40的偶整数

	//CQI相关
	double CQI;
	int CQI_index;
	
	//buffer相关
	int buffer_state;//初始时为in_prefetch，play时为in_play，starvation时为in_starvation
	double throughput_instant;//瞬时吞吐量，也是当前下载速率
	int number_of_chunk;

	double time_total_played;
	double total_played;
	int index_playing_current_chunk;
	double bitrate_playing_current_chunk;
	double size_playing_current_chunk;
	double played_current_chunk;

	double total_downloaded;
	int index_downloading_current_chunk;
	double bitrate_downloading_current_chunk;
	vector<double> bitrate_selected;
	int number_bitrate_switch;
	double size_downloading_current_chunk;
	double downloaded_current_chunk;

	double buffer;
	double starvation_left;

	

	//预测目标
	int y_output;//发生过卡顿就设置为1
	int num_starvation;
	double time_in_cell;
};

#endif