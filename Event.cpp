#include"Event.h"

Event::Event(double t, long i, type_of_event ty)
{
	time = t;
	index = i;
	type = ty;
}

bool Event::operator<(const Event& e) const
{
	if (this->time < e.time)//ʱ���������һ˵�𣿣�
		return true;
	else 
		return false;
}
