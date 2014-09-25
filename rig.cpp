#include <iostream>
#include <cmath>

#include "moremath.h"
#include "rig_options.h"
#include "rig_utils.h"

using namespace std;
int main(int iargs, const char ** args) {
	opt_t opt;
    read_options(opt,iargs,args);

    results_t results;
    switch(opt.in_fmt){
    case opt_t::IN_TREE:
	    results=parse_tree(opt.in);
	    break;
    case opt_t::IN_LIST:
	    results=parse_list(opt.in);
	    break;
    default:
	    throw runtime_error("Unimplemented Input Format.");
    }

	score_t scores;
	for(auto k:results){
		update_scores(scores,k.name1,k.name2,k.score1,k.score2,opt.cor);
	}

    switch(opt.out_fmt){
    case opt_t::OUT_ELO:
	    for(auto k:scores){
		    *(opt.out)<<k.second<<"\t"<<k.first<<std::endl;
	    }
	    break;
    case opt_t::OUT_LIST:
	    for(auto& k:results){
		    *(opt.out)<< k.name1 << " " << k.score1 <<" - "
		              << k.score2<< " " << k.name2 << std::endl;
	    }
	    break;
    default:
	    throw runtime_error("Unimplemented Output Format.");
    }
	return 0;
}
