#ifndef PAIR_H_
#define PAIR_H_

#include"fun.h"
#include"input.h"
#include<iostream>
using namespace std;

class Pair
{
private:
	Z_info z_pair;//beginning information
	Y_info y_pair;//ending information
public:
	Pair(const Z_info& argu_z, const Y_info& argu_y);
	Z_info z_pair_value() { return z_pair; }
	Y_info y_pair_value() { return y_pair; }
	friend ostream& operator<<(ostream& os, Pair& p); 
};

#endif