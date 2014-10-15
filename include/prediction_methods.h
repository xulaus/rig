#ifndef PREDICTION_METHODS_H
#define PREDICTION_METHODS_H
#include <functional>
#include "rig_utils.h"

class prediction_method{
	public:
		prediction_method();
		virtual score_t operator()(results_t) = 0;
		virtual ~prediction_method();
};

class elo : public prediction_method{
	private:
		std::function<double(double,double,int,int)> corrector;
	public:
		elo(std::function<double(double,double,int,int)>);
		score_t operator()(results_t);
};
#endif