#include "prediction_methods.h"

prediction_method::prediction_method(){}
prediction_method::~prediction_method(){}

elo::elo(std::function<double(double,double,int,int)> f): corrector(f){
}

score_t elo::operator()(results_t results){
	score_t elo_map;
	for(auto k:results){
		if(!elo_map.count(k.name1))
			elo_map[k.name1]=1400.0;

		if(!elo_map.count(k.name2))
			elo_map[k.name2]=1400.0;

		auto r1 = elo_map[k.name1];
		auto r2 = elo_map[k.name2];

		elo_map[k.name1] += corrector(r1,r2,k.score1,k.score2);
		elo_map[k.name2] += corrector(r2,r1,k.score2,k.score1);
	}
	return elo_map;
}