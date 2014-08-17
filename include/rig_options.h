#ifndef RIG_OPTIONS_H
#define RIG_OPTIONS_H
#include <iostream>

struct opt_t{
	std::istream* in;
	std::ostream* out;
	double (*cor)(double,double,int,int);
	opt_t();
	~opt_t();
};

void read_options(opt_t& opt,int iargs,const char ** arg);
#endif
