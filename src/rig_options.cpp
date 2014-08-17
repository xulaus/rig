#include <cstring>
#include <fstream>
#include "rig_options.h"
#include "rig_utils.h"

opt_t::opt_t():in(&std::cin),out(&std::cout),cor(&linear_correction){};
opt_t::~opt_t(){
	if(in!=&std::cin && out!=nullptr){
		delete in;
	}
	if(out!=&std::cout && out!=nullptr){
		delete out;
	}
}

void read_options(opt_t& opt,int iargs,const char ** arg){
	for(int i=1;i<iargs;i++){
		if(strcmp("-i",arg[i])==0){
			if(++i<iargs){
				opt.in = new std::ifstream(arg[i]);
				if(!opt.in->good()){
					std::cerr<<"Could not open '"<<arg[i]<<"'"<<std::endl;
					exit(1);
				}
			}
			else{
				std::cerr<<"No input file given after '-i'"<<std::endl;
				exit(1);
			}
		}
		else if(strcmp("-o",arg[i])==0){
			if(++i<iargs){
				opt.out = new std::ofstream(arg[i]);
				if(!opt.out->good()){
					std::cerr<<"Could not open '"<<arg[i]<<"'"<<std::endl;
					exit(1);
				}
			}
			else{
				std::cerr<<"No output file given after '-o'"<<std::endl;
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
			std::cerr<<"Unrecognised option '"<<arg[i]<<"'"<<std::endl;
			exit(1);
		}
	}
}
