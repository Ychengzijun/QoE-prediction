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

//��ͬpolicy�¼���throughput
	//����round_robin policyʱ��throughput
double throughput_round_robin(double cqi, long number_in_cell);
	//����max_min policyʱ��throughput
double throughput_maxmin(vector<long>& v_u);

//ָ���ֲ�������
double randomExponential(double lambda);//���ز���Ϊ1 / lambda��ָ���ֲ���doubleֵ//duration��arrival_time

//Event���
	//unique_ptr<Event>�ıȽ�
bool comp(unique_ptr<Event>& ep1, unique_ptr<Event>& ep2);

	//���Event��Ӧ��User��λ��
unique_ptr<User>& where_this_user(vector<unique_ptr<User>>& v_u, index_aimed i_a);

	//�������arrival�¼�������true
bool is_not_arrival(unique_ptr<Event>& e_p);

	//ɾ������arrival�¼��������¼�
void erase_outdated_event(vector<unique_ptr<Event>>& v_e);

	//����arrival�¼���ʱ�䣨��Ϊarrival�¼���һ����һ�������ģ�v_Ep����Զֻ��һ��arrival�¼���
void update_arrival_event(vector<unique_ptr<Event>>& v_e, double t_arrival);

	//����ÿ����ԾUser���¸��¼�
void throw_event(vector<unique_ptr<User>>& v_u, vector<unique_ptr<Event>>& v_e);

//ÿ���û���ϵͳ�е�ʱ��
void each_time_in_cell(vector<unique_ptr<User>>& v_u, double t_last);

//��¼User arrivalʱ��Ϣ
	//��Ϣ��ʽ
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

	long num_to_starvation;//�仯��С��0���û��� //С��0�ı����أ�ƽ��Ƿȱ��
	double feature_time_to_stay;//��ǰϵͳ���û�Ԥ��ͣ��ʱ����feature����ǰϵͳ���û�ƽ����ɱ���
	double feature_throughput_in_cell;//ϵͳƽ��������
	vector<long> flow_number_of_each_type;
	
};
bool comp_Z_info(const Z_info& z1, const Z_info& z2);
	//ͳ�ƺ���
	//ԭ���е�
vector<long> number_of_user_incell_eachregion();//����v_UpΪ����������number_of_user_incell_eachregion������vector
long number_of_user_instarvation(vector<unique_ptr<User>>& v);//����v_UpΪ����������number_of_user_instarvation������
vector<long> number_of_user_instarvation_eachregion(vector<unique_ptr<User>>& v);//����v_UpΪ����������number_of_user_instarvation_eachregion������vector
	//�Ҽӵ�
long cal_number_to_starvation(vector<unique_ptr<User>>& v);//��ǰϵͳ�У��п������Ƶ��û���
double cal_feature_time_to_stay(vector<unique_ptr<User>>& v);//��ǰϵͳ���û���ʣ��ʱ���һ��ָ��
double cal_feature_throughput_in_cell(vector<unique_ptr<User>>& v);//��ǰϵͳ��������feature
vector<long> number_of_user_incell_each_type();//��ǰϵͳ�и�����û���

Z_info record_z(int switch_button, User& u, vector<unique_ptr<User>>& v);//User arrivalʱ��¼User����Ϣ

//��¼ָ��������User��z_output
struct Y_info
{
	long y_index;
	int y_type;
	//ԭ�ĵ�
	int y_value;
	//�Ҽӵ�
	int y_num_starvation;
	double y_starvation_frequency;
	string y_starvation_frequency_label;
	double y_average_bitrate;
	string y_average_bitrate_label;
	double y_frequency_bitrate_switch;
	string y_frequency_bitrate_switch_label;
	
	//����
	double y_time_along;
};
bool comp_Y_info(const Y_info& y1, const Y_info& y2);
Y_info record_y(int switch_button, unique_ptr<User>& p_u);

//User���
	//��������ÿ����ԾUser��buffer��Ϣ
void update_other_buffer_infomation(vector<unique_ptr<User>>& v_Up, long index_last, double t_last_e);

	//����ÿ����ԾUser��throughput_instant
void update_throughput(vector<unique_ptr<User>>& v);

//���ͳ����Ϣ�����ݼ�
class Pair;
long count_starvation_of_vp(vector<Pair>& v_c);//ͳ�ƴ�v�з���starvation�ĸ���
vector<Pair>& delete_unstable_in_begin_end(int switch_button, vector<Pair>& v_d);//���ƶ�����ɾ����ʼ�ͽ���ʱ������
void output_train_test(vector<Pair>& v_p_d, const char* s_train, const char* s_test);//����vector�������ָ��������������ָ�����Ƶ�ѵ�����Ͳ��Լ���
double average_time_incell(vector<Pair>& v_p_arg);//ͳ����ϵͳ�е�ƽ��ʱ��
void output_whole(vector<Pair>& v_p_arg);//���all_User
void output_starvation(vector<Pair>& v_p_arg);//���all_starvation
vector<vector<Pair>> classify_into_show_each_type(vector<Pair>& v_p_arg);//���࣬����
void show_statistics(ostream& os, long total_user, vector<long>& number_each_type_arg, double average_time_arg, 
					 long total_starvation, vector<long>& number_each_type_starvation_arg);//���ͳ����Ϣ
void show_output(vector<Pair>& v_p_arg);//���ͳ����Ϣ�����ݼ�

//����ܵ����ݼ�
	//�����ָ���ļ�
void output_to_specified_file(vector<Pair>& v_p_d, const char* type_name);
	//�����ļ��ֳ����ֱַ����
void output_four_type(vector<Pair>& v_p_arg);
//��ʾ����ֵ�趨
void show_input();

#endif
