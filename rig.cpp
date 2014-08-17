#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <regex>
#include <cmath>

#include "moremath.h"
#include "rig_options.h"
#include "rig_utils.h"

using namespace std;
int main(int iargs, const char ** args) {
	opt_t opt;
    read_options(opt,iargs,args);

	string line;
	score_t scores;

    regex names_format("([a-z ]*) - ([a-z ]*)",regex_constants::icase);
    regex names_and_score_format("([a-z ]*) (\\d+) - (\\d+) ([a-z ]*)",regex_constants::icase);

    while(getline(*(opt.in),line)){
		smatch matches;

		if(regex_match(line, matches, names_format)){
			update_scores(scores,
			              matches[1].str(),
			              matches[2].str(),
			              1,1,
			              opt.cor);
		}
        else if(regex_match(line,matches, names_and_score_format)){
	        auto score = stoi(matches[2].str());
	        auto rounds= stoi(matches[3].str()) + score;
	        update_scores(scores,
	                      matches[1].str(),
	                      matches[4].str(),
	                      score, rounds,
	                      opt.cor);
        }
        else{
	        cerr<< "Datafile in incorrect format."<<endl;
	        exit(1);
        }

    }
    for(auto& k:scores){
	    *(opt.out)<<k.second<<"\t"<<k.first<<endl;
    }
	return 0;
}
