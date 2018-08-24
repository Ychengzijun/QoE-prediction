#include"User.h"
#include"input.h"
#include"fun.h"
#include<iostream>
#include<fstream>
#include<numeric>
using namespace std;

const double T_impossible = 10000;
ofstream fcout_process(".\\output_txt\\process of all.txt");

long User::number_of_user_incell = default_number_in_cell;
vector<long> User::v_each_type(4);
vector<long> User::v_each_region(number_of_CQI);

//User���
//�������User
User::User(long index_ctor, double t_c)	//t_c������show_process_information�Ĳ���
{
	//�û������
	number_of_user_incell++;
	
	//User���
	index = index_ctor;
	state = active;
	duration = (int (randomExponential(1 / T)) / h) * h;//doubleת��Ϊeven int�������
	if (duration == 0)
		duration = h;

	//CQI���
	CQI = CQI_ini();//CQI���ձ������������ѡȡ

	//buffer���
	buffer_state = in_prefetch;
	throughput_instant = throughput_instant_ini();

	switch (rand() % 4)//streaming type�������ķ�֮һ
	{
	case 0:
		type_of_streaming = static_adaptive;
		next_mobility = T_impossible;
		bitrate_downloading_current_chunk = bitrate_initial();//adaptive��User����ʼ��Ϊ1
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 1:
		type_of_streaming = mobile_adaptive;
		next_mobility = randomExponential(mobility_rate);
		bitrate_downloading_current_chunk = bitrate_initial();
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 2:
		type_of_streaming = static_fixed;
		next_mobility = T_impossible;
		bitrate_downloading_current_chunk = default_bitrate_fixed;//fixed��User����ʼ��Ϊ1.5
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 3:
		type_of_streaming = mobile_fixed;
		next_mobility = randomExponential(mobility_rate);
		bitrate_downloading_current_chunk = default_bitrate_fixed;
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	}
	v_each_type[type_of_streaming]++;

	number_of_chunk = duration / h;
	/*play*/
	time_total_played = 0;
	total_played = 0;
	index_playing_current_chunk = 1;
	bitrate_playing_current_chunk = bitrate_downloading_current_chunk;
	size_playing_current_chunk = bitrate_playing_current_chunk * h;
	played_current_chunk = 0;
	/*download*/
	total_downloaded = 0;
	index_downloading_current_chunk = 1;
	number_bitrate_switch = 0;
	size_downloading_current_chunk = h * bitrate_downloading_current_chunk;
	downloaded_current_chunk = 0;
	
	buffer = 0;
	starvation_left = B;

	//���
	y_output = -1;
	num_starvation = 0;
	time_in_cell = 0;
	show_process_information(hint_arrival, t_c, switch_show_process_information);
}
//�ֶ�����User
User::User(long index_ctor, int CQI_index_ctor, int type_ctor, int duration_ctor, double t_c)
{
	//�û������
	number_of_user_incell++;
	
	//User���
	index = index_ctor;
	state = active;
	duration = duration_ctor;
	if (duration == 0)
		duration = h;

	//CQI���
	CQI_index = CQI_index_ctor;
	switch (CQI_index_ctor)
	{
	case 0:
		CQI = CQIs[0].CQI_value;
		break;
	case 1:
		CQI = CQIs[1].CQI_value;
		break;
	case 2:
		CQI = CQIs[2].CQI_value;
		break;
	case 3:
		CQI = CQIs[3].CQI_value;
		break;
	case 4:
		CQI = CQIs[4].CQI_value;
		break;
	}

	//buffer���
	buffer_state = in_prefetch;
	throughput_instant = throughput_instant_ini();

	switch (type_ctor)//streaming type�������ķ�֮һ
	{
	case 0:
		type_of_streaming = static_adaptive;
		next_mobility = T_impossible;
		bitrate_downloading_current_chunk = bitrate_initial();//adaptive��User����ʼ��Ϊ1
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 1:
		type_of_streaming = mobile_adaptive;
		next_mobility = randomExponential(mobility_rate);
		bitrate_downloading_current_chunk = bitrate_initial();
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 2:
		type_of_streaming = static_fixed;
		next_mobility = T_impossible;
		bitrate_downloading_current_chunk = default_bitrate_fixed;//fixed��User����ʼ��Ϊ1.5
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	case 3:
		type_of_streaming = mobile_fixed;
		next_mobility = randomExponential(mobility_rate);
		bitrate_downloading_current_chunk = default_bitrate_fixed;
		bitrate_selected.push_back(bitrate_downloading_current_chunk);
		break;
	}
	v_each_type[type_of_streaming]++;

	number_of_chunk = duration / h;
	/*play*/
	time_total_played = 0;
	total_played = 0;
	index_playing_current_chunk = 1;
	bitrate_playing_current_chunk = bitrate_downloading_current_chunk;
	size_playing_current_chunk = bitrate_playing_current_chunk * h;
	played_current_chunk = 0;
	/*download*/
	total_downloaded = 0;
	index_downloading_current_chunk = 1;
	number_bitrate_switch = 0;
	size_downloading_current_chunk = h * bitrate_downloading_current_chunk;
	downloaded_current_chunk = 0;
	
	buffer = 0;
	starvation_left = B;

	//���
	y_output = -1;
	num_starvation = 0;
	time_in_cell = 0;

	show_process_information(hint_arrival, t_c, switch_show_process_information);
}
void User::into_unactive() 
{
	state = unactive;
	number_of_user_incell--;
	v_each_region[CQI_index]--;
	v_each_type[type_of_streaming]--;
}


//CQI���
double User::CQI_ini()//���Ը�һ�㻯
{
	double p = double((rand() % 1000)) / 1000;
	if (p < CQIs[0].CQI_pro)
		CQI_index = 0;
	else if (p < (CQIs[1].CQI_pro + CQIs[0].CQI_pro))
		CQI_index = 1;
	else if (p < (CQIs[2].CQI_pro + CQIs[1].CQI_pro + CQIs[0].CQI_pro))
		CQI_index = 2;
	else if (p < (CQIs[3].CQI_pro + CQIs[2].CQI_pro + CQIs[1].CQI_pro + CQIs[0].CQI_pro))
		CQI_index = 3;
	else
		CQI_index = 4;

	v_each_region[CQI_index]++;
	return CQIs[CQI_index].CQI_value;
}

void User::CQI_next()
{
	v_each_region[CQI_index]--;

	int i = std::pow(-1, rand() % 2);
	CQI_index += i;
	if (CQI_index == -1)
		CQI_index = number_of_CQI - 1;
	if (CQI_index == number_of_CQI)
		CQI_index = 0;

	v_each_region[CQI_index]++;
	CQI = CQIs[CQI_index].CQI_value;
}

//buffer���
double User::throughput_instant_ini()
{
	switch(current_policy)
	{
	case 0:
		return throughput_round_robin(CQI, number_of_user_incell);//����ʱ��number_of_user_incell��Զ������0��//����ڱ���v_Up���㷨����׼ȷ�����߼���throughputʱ��δ���Լ���������
		break;
	case 1:
		return throughput_maxmin(v_each_region);
		break;
	case 2:
		return 0;
		break;
	}

}

void User::throughput_instant_update()
{
	throughput_instant = throughput_instant_ini();
}

double User::bitrate_initial()
{
	if (throughput_instant >= fourth_bitrate_adaptive)
		bitrate_downloading_current_chunk = fourth_bitrate_adaptive;
	else if (throughput_instant >= third_bitrate_adaptive)
		bitrate_downloading_current_chunk = third_bitrate_adaptive;
	else if (throughput_instant >= second_bitrate_adaptive)
		bitrate_downloading_current_chunk = second_bitrate_adaptive;
	else
		bitrate_downloading_current_chunk = first_bitrate_adaptive;

	return bitrate_downloading_current_chunk;
}

void User::bitrate_downloading_current_chunk_update()
{
	double former = bitrate_downloading_current_chunk;
	if (throughput_instant >= fourth_bitrate_adaptive)
		bitrate_downloading_current_chunk = fourth_bitrate_adaptive;
	else if (throughput_instant >= third_bitrate_adaptive)
		bitrate_downloading_current_chunk = third_bitrate_adaptive;
	else if (throughput_instant >= second_bitrate_adaptive)
		bitrate_downloading_current_chunk = second_bitrate_adaptive;
	else
		bitrate_downloading_current_chunk = first_bitrate_adaptive;
	
	if (bitrate_downloading_current_chunk != former)
		number_bitrate_switch++;
}

void User::downloading_new_chunk()
{
	index_downloading_current_chunk++;
	size_downloading_current_chunk = h * bitrate_downloading_current_chunk;
	downloaded_current_chunk = 0;
	bitrate_selected.push_back(bitrate_downloading_current_chunk);
}

void User::play_about_information(double t_play)
{
	if (buffer_state == in_play)//����ղ��ڲ��š����û���������play����������ı�
	{
		time_total_played += t_play;
		index_playing_current_chunk = (int) time_total_played / h + 1;
		bitrate_playing_current_chunk = bitrate_selected[index_playing_current_chunk - 1];
		size_playing_current_chunk = h * bitrate_playing_current_chunk;

		played_current_chunk = (time_total_played - (index_playing_current_chunk - 1) * h) * bitrate_playing_current_chunk;
		total_played = 0;
		for (int i_buffer = 0; i_buffer < index_playing_current_chunk - 1; i_buffer++)
			total_played += (bitrate_selected[i_buffer] * h);
		total_played += played_current_chunk;
	}
}

void User::starvation_left_update(double t_s_l_u)
{
	if (starvation_left == 0)//ֻ�з���play_event���û�starvation_left == 0
		starvation_left = B;
	else
		if (buffer_state == in_prefetch || buffer_state == in_starvation)
			starvation_left = starvation_left - t_s_l_u;
		
}

void User::next_mobility_update(double t_n_m_u)
{
	if (next_mobility == 0)
		next_mobility = randomExponential(mobility_rate);
	else 
		if (type_of_streaming == mobile_adaptive || type_of_streaming == mobile_fixed)
			next_mobility -= t_n_m_u;
}

void User::buffer_infomation_update(double t_buffer)//�Ա���û�����Ϣ���£�����Ƕ��¼������ߵ��������¼������߾�����һ���¼�������û�û�о����µ��¼�
{
	//int buffer_state;
	//double throughput_instant;
	//int number_of_chunk;

	play_about_information(t_buffer);
	
	total_downloaded += throughput_instant * t_buffer;
	//index_downloading_current_chunk;ÿ��downloading_new_chunk()����
	//bitrate_downloading_current_chunk;ÿ��downloading_new_chunk()����
	//bitrate_selected;ÿ��get_event_chunkʱ����
	//size_downloading_current_chunk;��downloading_new_chunk()�и���
	//downloaded_current_chunk;downloading_new_chunk���������㣬
	downloaded_current_chunk += throughput_instant * t_buffer;


	buffer = total_downloaded - total_played;
	starvation_left_update(t_buffer);
	next_mobility_update(t_buffer);
}


double User::average_bitrate_value()
{
	if (bitrate_selected.size() == 0)
		return bitrate_playing_current_chunk;
	else
	{
		double sum = accumulate(bitrate_selected.begin(), bitrate_selected.end(), 0.0);
		return sum / bitrate_selected.size();
	}
}

double User::expect_time_to_stay()
{
	return (number_of_chunk - index_downloading_current_chunk) * average_bitrate_value() * 2 / throughput_instant;
}

//�¼�������
double User::time_until_chunk()
{
	return (size_downloading_current_chunk - downloaded_current_chunk) / throughput_instant;
}

double User::time_until_starvation()//���ص�ʱ�����Ǻ������
{
	if (buffer_state == in_prefetch || buffer_state == in_starvation)
		return T_impossible;
	double time_starvation = 0;
	if (index_downloading_current_chunk > (index_playing_current_chunk + 1))//�м�������һ��ʱ
	{
		for (int i_time = index_playing_current_chunk; i_time < index_downloading_current_chunk - 1; i_time++)//�м�chunk����
			time_starvation += bitrate_selected[i_time] * h;
		time_starvation = time_starvation +  (size_playing_current_chunk - played_current_chunk);//+��ǰ����chunk������
		time_starvation = time_starvation - bitrate_downloading_current_chunk * ((size_playing_current_chunk - played_current_chunk) / bitrate_playing_current_chunk + h * (index_downloading_current_chunk - 1 - index_playing_current_chunk));
		time_starvation = (time_starvation  - buffer) / (throughput_instant - bitrate_downloading_current_chunk);
		//cout << "first " << time_starvation << endl;
		return (time_starvation < 0) ? T_impossible : time_starvation;
	}
	else if (index_downloading_current_chunk == (index_playing_current_chunk + 1))
	{
		time_starvation = size_playing_current_chunk - played_current_chunk - buffer;
		time_starvation = time_starvation - bitrate_downloading_current_chunk * ((size_playing_current_chunk - played_current_chunk) / bitrate_playing_current_chunk);
		time_starvation = time_starvation / (throughput_instant - bitrate_downloading_current_chunk);
		//cout << "second " << time_starvation << endl;
		return (time_starvation < 0) ? T_impossible : time_starvation;
	}
	else
	{
		//cout << "third " << endl;
		return (bitrate_playing_current_chunk - throughput_instant < 0) ? T_impossible : buffer / (bitrate_playing_current_chunk - throughput_instant);
	}
}

double User::time_until_play()
{
	if (buffer_state == in_play)
		return T_impossible;
	return starvation_left;
}

double User::time_until_mobility()
{
	return next_mobility;
}

Event User::next_event()
{
	double t_chunk = time_until_chunk();
	double t_starvation = time_until_starvation();
	double t_play = time_until_play();
	double t_mobility = time_until_mobility();

	double t_return;
	Event::type_of_event type_return;

	switch(buffer_state)
	{
	case in_prefetch:
		if (t_play < t_chunk)
		{
			t_return = t_play;
			type_return =  Event::buffer_play;
		}
		else
		{
			t_return = t_chunk;
			type_return = (index_downloading_current_chunk == number_of_chunk) ? Event::departure : Event::chunk;
		}
		break;
	case in_play:
		if (t_starvation < t_chunk)
		{
			t_return = t_starvation;
			type_return =  Event::buffer_starvation;
		}
		else
		{
			t_return = t_chunk;
			type_return = (index_downloading_current_chunk == number_of_chunk) ? Event::departure : Event::chunk;
		}
		break;
	case in_starvation:
		if (t_play < t_chunk)
		{
			t_return = t_play;
			type_return = Event::buffer_play;
		}
		else
		{
			t_return = t_chunk;
			type_return = (index_downloading_current_chunk == number_of_chunk) ? Event::departure : Event::chunk;
		}
	}
	if (t_mobility < t_return)
	{
		t_return = t_mobility;
		type_return = Event::mobility;
	}

	return Event(t_return, index, type_return);
}

//�¼���������״̬ԾǨ��������������(���ϣ�����)
void User::get_event_departure(double t_get_departure)
{
	show_process_information(hint_departurel, t_get_departure, switch_show_process_information);
	play_about_information(t_get_departure);
	starvation_left_update(t_get_departure);
	next_mobility_update(t_get_departure);
	total_downloaded = 0;
	for (int i_buffer = 0; i_buffer < index_downloading_current_chunk; i_buffer++)
			total_downloaded += (bitrate_selected[i_buffer] * h);
	buffer = total_downloaded - total_played;
	downloaded_current_chunk = size_downloading_current_chunk;
	
	into_unactive();
	show_buffer_information(switch_show_buffer_information);
}

void User::get_event_chunk(double t_get_chunk)//�ǲ��ı�״̬��
{
	show_process_information(hint_chunk, t_get_chunk, switch_show_process_information);
	play_about_information(t_get_chunk);
	starvation_left_update(t_get_chunk);
	next_mobility_update(t_get_chunk);
	total_downloaded = 0; 
	for (int i_buffer = 0; i_buffer < index_downloading_current_chunk; i_buffer++)
			total_downloaded += (bitrate_selected[i_buffer] * h);
	buffer = total_downloaded - total_played;
	
	if (type_of_streaming == static_adaptive || type_of_streaming == mobile_adaptive)
		bitrate_downloading_current_chunk_update();
	downloading_new_chunk();//��ʼ��chunk����ʼ����ǰchunk�Ĳ���
	
	//cout << "\tnew chunk with bitrate: " << bitrate_downloading_current_chunk << endl;
	//fcout_process << "\tnew chunk with bitrate: " << bitrate_downloading_current_chunk << endl;

	show_buffer_information(switch_show_buffer_information);
}

void User::get_event_starvation(double t_starvation)//��in_play״̬���ģ���buffer�ľ��Ľ��
{
	show_process_information(hint_starvation, t_starvation, switch_show_process_information);
	play_about_information(t_starvation);
	
	total_downloaded = total_played;
	downloaded_current_chunk = played_current_chunk;//

	buffer = 0;
	starvation_left_update(t_starvation);
	next_mobility_update(t_starvation);

	y_output = 1;
	num_starvation++;
	buffer_state = in_starvation;

	show_buffer_information(switch_show_buffer_information);
}

void User::get_event_play(double t_play)//��in_prefetch��in_starvation����
{
	show_process_information(hint_play, t_play, switch_show_process_information);
	starvation_left_update(t_play);
	downloaded_current_chunk += throughput_instant * t_play;
	total_downloaded = 0; 
	if (index_downloading_current_chunk == 1)
		total_downloaded = downloaded_current_chunk;
	else 
	{
		for (int i_buffer = 0; i_buffer < index_downloading_current_chunk - 1; i_buffer++)
			total_downloaded += (bitrate_selected[i_buffer] * h);
		total_downloaded += downloaded_current_chunk;
	}
	
	buffer = total_downloaded - total_played;
	starvation_left_update(t_play);
	next_mobility_update(t_play);
	
	buffer_state = in_play;

	show_buffer_information(switch_show_buffer_information);
}

void User::get_event_mobility(double t_mobility)//ֻ�з�����event_mobility�Ż�����µ�next_mobility���Ż�throw�µ�event_mobility
{
	show_process_information(hint_mobility, t_mobility, switch_show_process_information);
	//����
	buffer_infomation_update(t_mobility);//������һ��next_mobility_update(),ʹnext_mobility��0
	//����
	next_mobility_update(t_mobility);//�����µ�next_mobility
	CQI_next();
	throughput_instant_update();
	show_buffer_information(switch_show_buffer_information);
}

//Ԥ��Ŀ��
string User::starvation_frequency_label()
{
	string label;
	double frequency = starvation_frequency_value();
	if (frequency == 0)
		label = "RF1";
	else if (frequency < treshold_starvation_frequency)
		label = "RF2";
	else
		label = "RF3";

	return label;
}

string User::average_bitrate_label()
{
	string label;
	double average = average_bitrate_value();
	if (average <= treshold_average_bitrate_first)
		label = "MB1";
	else if (average <= treshold_average_bitrate_second)
		label = "MB2";
	else
		label = "MB3";

	return label;
}

string User::frequency_bitrate_switch_label()
{
	string label;
	double frequency = frequency_bitrate_switch_value();
	if (type_of_streaming == static_adaptive || type_of_streaming == static_fixed)
	{
		if (frequency == 0)
			label = "SF1";
		else if (frequency < treshold_bitrate_switch_static)
			label = "SF2";
		else
			label = "SF3";
	}
	else
	{
		if (frequency == 0)
			label = "SF1";
		else if (frequency < treshold_bitrate_switch_mobile)
			label = "SF2";
		else
			label = "SF3";
	}

	return label;
}

//��ʾ��
void User::show_buffer_information(int switch_button)
{
	if (switch_button)
	{
		fcout_process << "CQI_index: " << CQI_index << endl;
		fcout_process << "buffer_state: " << buffer_state << endl;
		fcout_process << "throughput_instant: " << throughput_instant << endl;
		fcout_process << "number_of_chunk: " << number_of_chunk << endl;
		fcout_process << "\tAbout play: \n";
		fcout_process << "time_total_played: " << time_total_played << endl;
		fcout_process << "total_played: " << total_played << endl;
		fcout_process << "index_playing_current_chunk: " << index_playing_current_chunk << endl;
		fcout_process << "bitrate_playing_current_chunk: " << bitrate_playing_current_chunk << endl;
		fcout_process << "size_playing_current_chunk: " << size_playing_current_chunk << endl;
		fcout_process << "played_current_chunk: " << played_current_chunk << endl;
		fcout_process << "\tAbout download: \n";
		fcout_process << "total_downloaded: " << total_downloaded << endl;
		fcout_process << "index_downloading_current_chunk: " << index_downloading_current_chunk << endl;
		fcout_process << "bitrate_downloading_current_chunk: " << bitrate_downloading_current_chunk << endl;
		//fcout_process << "" << 	bitrate_selected;
		fcout_process << "size_downloading_current_chunk: " << size_downloading_current_chunk << endl;
		fcout_process << "downloaded_current_chunk: " << downloaded_current_chunk << endl;

		fcout_process << "buffer: " << buffer << endl;
		fcout_process << "starvation_left: " << starvation_left << endl;
		fcout_process << "next_mobility: " << next_mobility << endl;
	}
}

void User::show_process_information(type_process_hint t_p_h, double t_hint, int switch_button)
{
	if (switch_button)
	{
		cout << "After an interval of: " << t_hint << "\t";
		fcout_process << "After an interval of : " << t_hint << "\t";
		switch (t_p_h)
		{
		case hint_arrival:
			cout << "User " << index << " Arrived with ";
			cout << CQI_value() << " | " << duration << " | " << type_of_streaming << endl;
			fcout_process << "User " << index << " Arrived with ";
			fcout_process << CQI_value() << " | " << duration << " | " << type_of_streaming << endl;
			break;
		case hint_departurel:
			cout << index <<" get a departure.\n";
			cout << "\t\tUser " << index << " Left." << endl;
			fcout_process << index <<" get a departure.\n";
			fcout_process << "\t\tUser " << index << " Left." << endl;
			break;
		case hint_chunk:
			cout << index <<" get a chunk.\n";
			fcout_process << index <<" get a chunk.\n";
			break;
		case hint_play:
			cout << index <<" get a play.\n";
			fcout_process << index <<" get a play.\n";
			break;
		case hint_starvation:
			cout << index <<" get a starvation.\n";
			fcout_process << index <<" get a starvation.\n";
			break;
		case hint_mobility:
			cout << index <<" get a mobility.\n";
			fcout_process << index <<" get a mobility.\n";
			break;
		}
	}
}

void User::show_number_each_type()
{
	for (int i_s = 0; i_s < 4; i_s++)
		cout << v_each_type[i_s] << endl;
}
