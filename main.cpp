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
	vector<Pair> v_P;//����User����Ϣpair

	double t_next_arrival;//�����´�arrival�¼���ʱ����

	double t_last_e;//��ǰ�¼�ռ�ݵ�ʱ����
	int type_of_e;
	long index_of_e;

	long j = 0;
	while (v_Ep.size() != 0)//�����¼�Ҫ����
	{
		//ѡ����ǰ�¼�
		auto iterator_e = min_element(v_Ep.begin(), v_Ep.end(), comp);//���·������¼�
		t_last_e = (*iterator_e)->time_value();//�¼���Ӧ��User��Ҫ���t;�����û���Ҫ���t
		type_of_e = (*iterator_e)->type_value();//���µ��¼�������
		index_of_e = (*iterator_e)->index_value();//���µ��¼������ĸ��û�
		vector<unique_ptr<User>>::iterator iterator_u;//*iterator_u��v_Up�У�ָ��ǰ�¼���Ӧ��User
		if (type_of_e != Event::arrival)//�������arrival�¼����ҵ�����¼���Ӧ��User����switch���ʹ��
			iterator_u = find_if(v_Up.begin(), v_Up.end(), index_aimed(index_of_e));
		v_Ep.erase(iterator_e);//ɾ������¼��������arrival��Ҫɾ��������arrival�Ļ������Եȵ�erase_outdated_event����ʱɾ��
		
		each_time_in_cell(v_Up, t_last_e);//Ҳ�����ڳ�Ա����ʵ��
		
		//�����¼����ͣ�ѡ��ͬ�Ĵ���
		switch (type_of_e)
		{
		case Event::arrival:
			v_Up.push_back(unique_ptr<User>(new User(j, t_last_e)));//���û���ʽ����cell
			//v_Up.push_back(unique_ptr<User>(new User(0, 0, 0, 12, 0)));//index_ctor, CQI_index_ctor, type_ctor, duration_ctor, time_interval//{static_adaptive, mobile_adaptive, static_fixed, mobile_fixed}
			v_Z.push_back(record_z(switch_record_my_features, **(v_Up.end() - 1), v_Up));//recordӦ����v_Up.push_back֮ǰ��
			
			//�����¸�User�ĵ���
			if (j < l - 1)
			{
				j++;//�¸�������ǵ�j���û�
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
		if (type_of_e != Event::arrival && v_Ep.size() != 0)//���:�ղ�����¼�����arrival�¼�,���һ���arrival�¼�������
			update_arrival_event(v_Ep, t_last_e);
		throw_event(v_Up, v_Ep);
	}

	//���ϵ�Pair
	sort(v_Z.begin(), v_Z.end(), comp_Z_info);
	sort(v_Y.begin(), v_Y.end(), comp_Y_info);
	for (long i_p = 0; i_p < l; i_p++)
		v_P.push_back(Pair(v_Z[i_p], v_Y[i_p]));

	//���
	delete_unstable_in_begin_end(switch_delete_unstable_data, v_P);
	//show_output(v_P);
	output_four_type(v_P);
	show_input();

	clock_t end = clock();
	cout << "Run time: " << (double)(end - start)/CLOCKS_PER_SEC << endl;

	cout << "OK." << endl;
	return 0;
}


