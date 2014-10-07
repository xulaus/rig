#include <cstring>
#include <fstream>
#include "rig_options.h"
#include "rig_utils.h"

opt_t::opt_t():
	in(&std::cin),out(&std::cout),
	cor(&linear_correction),
	player1(),player2(),
	in_fmt(IN_TREE),out_fmt(OUT_ELO){}

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
		else if(strcmp("--itree",arg[i])==0){
			opt.in_fmt = opt_t::IN_TREE;
		}
		else if(strcmp("--ilist",arg[i])==0){
			opt.in_fmt = opt_t::IN_LIST;
		}
		else if(strcmp("--ielo",arg[i])==0){
			opt.in_fmt = opt_t::IN_ELO;
		}
		else if(strcmp("--olist",arg[i])==0){
			opt.out_fmt = opt_t::OUT_LIST;
		}
		else if(strcmp("--oelo",arg[i])==0){
			opt.out_fmt = opt_t::OUT_ELO;
		}
		else if(strcmp("--ocomp",arg[i])==0 && i+2 < iargs){
			opt.out_fmt = opt_t::OUT_COMP;
			opt.player1 = arg[++i];
			opt.player2 = arg[++i];
		}
		else if(strcmp("-?",arg[i])==0){
			std::cout<<
				"\t-?      Print this help message.\n"
				"\t-l      Use linear rating corrections (standard Elo).\n"
				"\t-b      Use binomial rating corrections.\n"
				"\t-i      Input file.\n"
				"\t-o      Output file.\n"
				"\t--itree Input is in tree format.\n"
				"\t--ilist Input is in a list format.\n"
				"\t--ielo  Input is a list of elo ratings.\n"
				"\t--olist Output wanted is a list of match results.\n"
				"\t--oelo  Output wanted is a list of ratings.";
			std::cout<<std::endl;
			exit(0);
		}
		else{
			std::cerr<<"Unrecognised option '"<<arg[i]<<"'"<<std::endl;
			exit(1);
		}
	}
}
