#include <cmath>
#include "moremath.h"
#include "rig_utils.h"

double linear_correction(double s1, double s2, int r1, int r2){
	auto s = (double) r1/(r1+r2);
    auto K = 16;
    auto e = 1/(1+pow(10,(s1-s2)/400.0));
    return K*(s-e);
}

double binomial_correction(double s1, double s2, int r1, int r2){
	auto total = r1+r2;
    auto p = 1/(1+pow(10,(s1-s2)/400.0));
    auto K = 32;

    double cor;
	cor=binomial_cdf(r1,p,total)+binomial_pdf(r1,p,total)/2-0.5;
    return K*cor;
}

void update_scores(score_t &elo_map,
                const std::string& k1, const std::string& k2,
                double s, double r,
                double (*corrector)(double,double,int,int)){
	if(!elo_map.count(k1))
		elo_map[k1]=1400.0;

	if(!elo_map.count(k2))
		elo_map[k2]=1400.0;

	auto s1 = elo_map[k1];
	auto s2 = elo_map[k2];

	elo_map[k1] += corrector(s1,s2,s,r-s);
	elo_map[k2] += corrector(s2,s1,r-s,s);
}
