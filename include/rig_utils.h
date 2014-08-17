#ifndef RIG_UTILS_H
#define RIG_UTILS_H
#include <map>
#include <string>

using score_t = std::map<std::string,double>;

double linear_correction(double s1, double s2, int r1, int r2);

double binomial_correction(double s1, double s2, int r1, int r2);

void update_scores(score_t &elo_map,
                   const std::string& k1, const std::string& k2,
                   double s, double r,
                   double (*corrector)(double,double,int,int));

#endif
