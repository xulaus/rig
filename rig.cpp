#include <iostream>
#include <cmath>

#include "moremath.h"
#include "rig_options.h"
#include "rig_utils.h"
#include "parsing.h"
#include "prediction_methods.h"

using namespace std;
int main(int iargs, const char ** args) {
	// Get options from command line
	opt_t opt;
	read_options(opt,iargs,args);

	// Generate a list of match outcomes from input
	results_t results;
	try{
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
	}
	catch (std::exception& e){
		cout<< "Parsing error! Error was:" << endl
		    << e.what() << endl;
		return 1;
	}

	// Calculate Elo Ratings
	score_t scores = elo(opt.cor)(results);

	// Output in relevant format
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
	case opt_t::OUT_COMP:{
		// If comparing two players
		// Get relevant players
		auto player1 = find_player(scores,opt.player1);
		auto player2 = find_player(scores,opt.player2);

		// Check players exist
		if(player1 == scores.end()){
			std::cerr << "Could not find player '" <<opt.player1<<"'"<<std::endl;
			return 1;
		}
		if(player2 == scores.end()){
			std::cerr << "Could not find player '" <<opt.player2<<"'"<<std::endl;
			return 1;
		}


		// Calculate win chance
		double e = expected_outcome(player1->second,player2->second);

		// If we are using binomial corrections we can use the CDF to more accurately determine win chance
		if(opt.cor == binomial_correction){
			e = 1-binomial_cdf(2,e,5);
		}

		// If more likely to loose, swap players so player1 is winner
		if(e<0.5){
			swap(player1,player2);
			e = 1-e;
		}

		// Output result
		*(opt.out)<< player1->first << " to beat " << player2->first << " (" << (e*100) << "%)"<<std::endl;
		}
		break;
	default:
		throw runtime_error("Unimplemented Output Format.");
	}
	return 0;
}
