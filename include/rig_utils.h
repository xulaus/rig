#ifndef RIG_UTILS_H
#define RIG_UTILS_H

#include <map>
#include <string>
#include <queue>

using score_t = std::map<std::string,double>;
struct outcome_t{
	std::string name1;
	std::string name2;
	int score1;
	int score2;
	outcome_t(std::string,std::string, int, int);
};

double expected_outcome(double s1,double s2);

score_t::iterator find_player(score_t&, std::string);


bool match_name(std::string, std::string);

double linear_correction(double s1, double s2, int r1, int r2);

double binomial_correction(double s1, double s2, int r1, int r2);

void update_scores(score_t &elo_map,
                   const std::string& k1, const std::string& k2,
                   double s, double r,
                   double (*corrector)(double,double,int,int));

#endif
