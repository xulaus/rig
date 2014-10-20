#include <cmath>
#include <utility>
#include <algorithm>
#include <regex>
#include "moremath.h"
#include "rig_utils.h"

outcome_t::outcome_t(std::string p1,std::string p2, int s1, int s2, unsigned int):
	name1(std::move(p1)),
	name2(std::move(p2)),
	score1(s1),
	score2(s2),
	time(1){}

double expected_outcome(double s1,double s2){
	return 1/(1+pow(10,(s2-s1)/400.0));
}

score_t::iterator find_player(score_t &elo_map, const std::string& x){
	return std::find_if(elo_map.begin(),elo_map.end(),[&](score_t::value_type y){
		return match_name(x,y.first);
	});
}

double linear_correction(double s1, double s2, int r1, int r2){
	if(r1+r2==0) return 0;
	auto s = (double) r1/(r1+r2);
	auto K = 16;
	auto e = expected_outcome(s1,s2);
	return K*(s-e);
}

double binomial_correction(double s1, double s2, int r1, int r2){
	if(r1+r2==0) return 0;
	auto total = r1+r2;
	auto p = expected_outcome(s1,s2);
	auto K = 32;

	double cor=binomial_cdf(r1-1,p,total)+binomial_pdf(r1,p,total)/2-0.5;
	return K*cor;
}

bool match_name(std::string y,const std::string& x){
	std::size_t i = 0;
	// We want to convert y to a regex that will match even if abbreviated
	// To do this we need to replace character that represent abbreviations with
	// an expression for with any potentially abbreviated letter. Spaces
	// may be needed if multiple names have been abbreviated to one letter
	std::string expansion_regex = "[A-Za-z\\- ]*";

	// Loop through replacing expansion characters with regex
	while((i=y.find_first_of("-.",i))!=std::string::npos){
		y.replace(i,1,expansion_regex);
		i+=expansion_regex.size()+1;
	}

	// used generated regex to see if match or not
	return std::regex_match(x,std::regex(y));
}
