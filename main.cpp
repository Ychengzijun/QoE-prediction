#include"input.h"
#include"User.h"
#include"Event.h"
#include"Pair.h"
#include"fun.h"
#include<iostream>
#include<algorithm>
#include<vector>
#include<memory>
#include<fstream>
#include<ctime>
using namespace std;

int main()
{
	//srand(time(0));

	clock_t start = clock();

	vector<unique_ptr<User>> v_Up;
	vector<Z_info> v_Z;
	vector<Y_info> v_Y;
	vector<unique_ptr<Event>> v_Ep;
	v_Ep.push_back(unique_ptr<Event>(new Event(0, 0, Event::arrival)));
	vector<Pair> v_P;//所有User的信息pair

	double t_next_arrival;//距离下次arrival事件的时间间隔

	double t_last_e;//当前事件占据的时间间隔
	int type_of_e;
	long index_of_e;

	long j = 0;
	while (v_Ep.size() != 0)//还有事件要发生
	{
		//选出当前事件
		auto iterator_e = min_element(v_Ep.begin(), v_Ep.end(), comp);//眼下发生的事件
		t_last_e = (*iterator_e)->time_value();//事件对应的User需要这个t;其他用户需要这个t
		type_of_e = (*iterator_e)->type_value();//眼下的事件的类型
		index_of_e = (*iterator_e)->index_value();//眼下的事件来自哪个用户
		vector<unique_ptr<User>>::iterator iterator_u;//*iterator_u在v_Up中，指向当前事件对应的User
		if (type_of_e != Event::arrival)//如果不是arrival事件，找到这个事件对应的User。供switch语句使用
			iterator_u = find_if(v_Up.begin(), v_Up.end(), index_aimed(index_of_e));
		v_Ep.erase(iterator_e);//删除这个事件。如果是arrival，要删除；不是arrival的话，可以等到erase_outdated_event（）时删除
		
		each_time_in_cell(v_Up, t_last_e);//也可以在成员函数实现
		
		//根据事件类型，选择不同的处理
		switch (type_of_e)
		{
		case Event::arrival:
			v_Up.push_back(unique_ptr<User>(new User(j, t_last_e)));//新用户正式进入cell
			//v_Up.push_back(unique_ptr<User>(new User(0, 0, 0, 12, 0)));//index_ctor, CQI_index_ctor, type_ctor, duration_ctor, time_interval//{static_adaptive, mobile_adaptive, static_fixed, mobile_fixed}
			v_Z.push_back(record_z(switch_record_my_features, **(v_Up.end() - 1), v_Up));//record应该在v_Up.push_back之前吗
			
			//安排下个User的到达
			if (j < l - 1)
			{
				j++;//下个到达的是第j个用户
				t_next_arrival = randomExponential(arrival_rate_max * arrival_load_current);
				//cout << "t_next_arrival: " << t_next_arrival << endl;
				v_Ep.push_back(unique_ptr<Event>(new Event(t_next_arrival, j, Event::arrival)));
			}
			break;
		case Event::departure:
			(*iterator_u)->get_event_departure(t_last_e);
			v_Y.push_back(record_y(switch_record_my_targets, *iterator_u));
			v_Up.erase(iterator_u);
			break;
		case Event::chunk:
			(*iterator_u)->get_event_chunk(t_last_e);
			break;
		case Event::buffer_starvation:
			(*iterator_u)->get_event_starvation(t_last_e);
			break;
		case Event::buffer_play:
			(*iterator_u)->get_event_play(t_last_e);
			break;
		case Event::mobility:
			(*iterator_u)->get_event_mobility(t_last_e);
			break;
		}
		
		update_other_buffer_infomation(v_Up, index_of_e, t_last_e);
		update_throughput(v_Up);

		erase_outdated_event(v_Ep);
		if (type_of_e != Event::arrival && v_Ep.size() != 0)//如果:刚才这个事件不是arrival事件,并且还有arrival事件待处理
			update_arrival_event(v_Ep, t_last_e);
		throw_event(v_Up, v_Ep);
	}

	//整合到Pair
	sort(v_Z.begin(), v_Z.end(), comp_Z_info);
	sort(v_Y.begin(), v_Y.end(), comp_Y_info);
	for (long i_p = 0; i_p < l; i_p++)
		v_P.push_back(Pair(v_Z[i_p], v_Y[i_p]));

	//输出
	delete_unstable_in_begin_end(switch_delete_unstable_data, v_P);
	//show_output(v_P);
	output_four_type(v_P);
	show_input();

	clock_t end = clock();
	cout << "Run time: " << (double)(end - start)/CLOCKS_PER_SEC << endl;

	cout << "OK." << endl;
	return 0;
}


