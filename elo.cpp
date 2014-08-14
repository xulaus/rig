#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <regex>
#include <cmath>

using namespace std;
using elo_t = map<string,double>;// as elo_t;

void update_elo(elo_t &elo_map,
                const string& k1, const string& k2,
                double s, double rounds=1){
    if(!elo_map.count(k1))
        elo_map[k1]=1400.0;

    if(!elo_map.count(k2))
        elo_map[k2]=1400.0;

    auto s1 = elo_map[k1];
    auto s2 = elo_map[k2];
    s=s/rounds;
    auto K = 16;
    auto e = 1/(1+pow(10,(s1-s2)/400.0));

    elo_map[k1] = s1 + K*(s-e);
    elo_map[k2] = s2 + K*(e-s);
}

struct opt_t{
	istream* in;
	ostream* out;
	opt_t():in(&cin),out(&cout){};
	~opt_t(){
		if(in!=&cin){
			delete in;
		}
		if(out!=&cout){
			delete out;
		}
	}
};

void read_options(opt_t& opt,int iargs,const char ** arg){
	for(int i=1;i<iargs;i++){
		if(strcmp("-i",arg[i])==0){
			if(++i<iargs){
				opt.in = new ifstream(arg[i]);
				if(!opt.in->good()){
					cerr<<"Could not open '"<<arg[i]<<"'"<<endl;
					exit(1);
				}
			}
			else{
				cerr<<"No input file given after '-i'"<<endl;
				exit(1);
			}
		}
		else if(strcmp("-0",arg[i])==0){
			if(++i<iargs){
				opt.out = new ofstream(arg[i]);
				if(!opt.out->good()){
					cerr<<"Could not open '"<<arg[i]<<"'"<<endl;
					exit(1);
				}
			}
			else{
				cerr<<"No output file given after '-o'"<<endl;
				exit(1);
			}
		}
		else{
			cerr<<"Unrecognised option '"<<arg[i]<<"'"<<endl;
			exit(1);
		}
	}
}

int main(int iargs, const char ** args) {
	opt_t opt;
    read_options(opt,iargs,args);

	string line;
	elo_t elo_map;

    regex names_format("([a-z ]*) - ([a-z ]*)",regex_constants::icase);
    regex names_and_score_format("([a-z ]*) (\\d+) - (\\d+) ([a-z ]*)",regex_constants::icase);

    while(getline(*(opt.in),line)){
		smatch matches;

		if(regex_match(line, matches, names_format)){
            update_elo(elo_map,matches[1].str(),matches[2].str(),1);
		}
        else if(regex_match(line,matches, names_and_score_format)){
	        auto score = stoi(matches[2].str());
	        auto rounds= stoi(matches[3].str()) + score;
	        update_elo(elo_map,matches[1].str(),matches[4].str(),score,rounds);
        }
        else{
			cerr<< "Datafile in incorrect format."<<endl<<line;
			exit(1);
		}

	}
    for(auto& k:elo_map){
	    *(opt.out)<<k.second<<"\t"<<k.first<<endl;
    }
	return 0;
}
