#include"Event.h"

Event::Event(double t, long i, type_of_event ty)
{
	time = t;
	index = i;
	type = ty;
}

bool Event::operator<(const Event& e) const
{
	if (this->time < e.time)//时间有相等这一说吗？？
		return true;
	else 
		return false;
}
