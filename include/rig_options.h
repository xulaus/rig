#ifndef RIG_OPTIONS_H
#define RIG_OPTIONS_H
#include <iostream>

struct opt_t{
	std::istream* in;
	std::ostream* out;
	std::string player1;
	std::string player2;
	double (*cor)(double,double,int,int);
	enum in_fmt_t{
		IN_TREE,
		IN_LIST,
		IN_ELO
	} in_fmt;
	enum out_fmt_t{
		OUT_ELO,
		OUT_LIST,
		OUT_COMP
	} out_fmt;
	enum method_t{
		ELO_METHOD
	} method;
	opt_t();
	~opt_t();
};

void read_options(opt_t& opt,int iargs,const char ** arg);
#endif
