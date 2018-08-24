#ifndef Event_H_
#define Event_H_

#include"User.h"//Event�Ƿ���Դ�User�̳ж�����Event����ֱ��ʹ��User����Ϣ�ˣ�oopʱ��˼������֮��Ĺ�ϵ�ǲ������ȿ��ǵ���Ҫ˼·֮һ

class Event
{
public:
	enum type_of_event{arrival, departure, chunk, buffer_play, buffer_starvation, buffer_prefetch, mobility};

	Event(double t, long i, type_of_event type);//ʱ�䣬����������//Event������ʱ����ô��ֵ
	double time_value() { return time; }
	void time_update(double t_reduce) { time -= t_reduce; }
	bool operator<(const Event& e) const;
	long index_value() {return index; }
	type_of_event type_value() { return type; }
	
private:
	double time;//���ʱ��������ôѡ������
	long index;//������User
	type_of_event type;//�¼����ͣ�Ӧ����Ϊ���캯���Ĳ���
};

#endif