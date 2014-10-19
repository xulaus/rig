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
	unsigned int time;
	outcome_t(std::string,std::string, int, int, unsigned int);
};
using results_t = std::vector<outcome_t>;

double expected_outcome(double s1,double s2);

score_t::iterator find_player(score_t&, const std::string&);


bool match_name(std::string, const std::string&);

double linear_correction(double s1, double s2, int r1, int r2);

double binomial_correction(double s1, double s2, int r1, int r2);


#endif
