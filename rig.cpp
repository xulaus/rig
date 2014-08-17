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


double linear_correction(double s1, double s2, int r1, int r2){
	auto s = (double) r1/(r1+r2);
    auto K = 16;
    auto e = 1/(1+pow(10,(s1-s2)/400.0));
    return K*(s-e);
}

double choose(double n, double k) {
	double r = 1;
	double d;
	if (k > n) return 0;
	for (d=1; d <= k; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}
double binomial_pdf(int x,double p, int n){
	return choose(n,x) * pow(p,x) * pow(1-p,n-x);
}
double binomial_cdf(int x,double p, int n){
	double sum=0;
	for(int i=0;i<x;i++){
		sum+= binomial_pdf(i,p,n);

	}
	return sum;
}


double binomial_correction(double s1, double s2, int r1, int r2){
	auto total = r1+r2;
    auto p = 1/(1+pow(10,(s1-s2)/400.0));
    auto K = 32;

    double cor;
	cor=binomial_cdf(r1,p,total)+binomial_pdf(r1,p,total)/2-0.5;
    return K*cor;
}

void update_elo(elo_t &elo_map,
                const std::string& k1, const std::string& k2,
                int s, int r,
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

struct opt_t{
	istream* in;
	ostream* out;
	double (*cor)(double,double,int,int);
	opt_t():in(&cin),out(&cout),cor(&linear_correction){};
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
		else if(strcmp("-b",arg[i])==0){
			opt.cor=&binomial_correction;
		}
		else if(strcmp("-l",arg[i])==0){
			opt.cor=&linear_correction;
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
            update_elo(elo_map,
                       matches[1].str(),matches[2].str(),
                       1,1,
                       opt.cor);
		}
        else if(regex_match(line,matches, names_and_score_format)){
	        auto score = stoi(matches[2].str());
	        auto rounds= stoi(matches[3].str()) + score;
	        update_elo(elo_map,
	                   matches[1].str(),matches[4].str(),
	                   score,rounds,
	                   opt.cor);
        }
        else{
			cerr<< "Datafile in incorrect format."<<endl;
			exit(1);
		}

	}
    for(auto& k:elo_map){
	    *(opt.out)<<k.second<<"\t"<<k.first<<endl;
    }
	return 0;
}
