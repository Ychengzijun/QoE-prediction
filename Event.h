#ifndef Event_H_
#define Event_H_

#include"User.h"//Event是否可以从User继承而来，Event可以直接使用User的信息了，oop时候思考对象之间的关系是不是优先考虑的重要思路之一

class Event
{
public:
	enum type_of_event{arrival, departure, chunk, buffer_play, buffer_starvation, buffer_prefetch, mobility};

	Event(double t, long i, type_of_event type);//时间，索引，类型//Event发生的时间怎么赋值
	double time_value() { return time; }
	void time_update(double t_reduce) { time -= t_reduce; }
	bool operator<(const Event& e) const;
	long index_value() {return index; }
	type_of_event type_value() { return type; }
	
private:
	double time;//这个时间粒度怎么选？？？
	long index;//所属的User
	type_of_event type;//事件类型，应当作为构造函数的参数
};

#endif