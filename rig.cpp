#include <iostream>
#include <cmath>

#include "moremath.h"
#include "rig_options.h"
#include "rig_utils.h"
#include "parsing.h"
#include "prediction_methods.h"

using namespace std;

results_t get_results(const opt_t& opt){
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
	return results;
}

void do_comparison(opt_t& opt,score_t scores){
		// If comparing two players
		// Get relevant players
		auto player1 = find_player(scores,opt.player1);
		auto player2 = find_player(scores,opt.player2);

		// Check players exist
		if(player1 == scores.end()){
			throw runtime_error("Could not find player '" + opt.player1 + "'");
		}

		if(player2 == scores.end()){
			throw runtime_error("Could not find player '" + opt.player2 + "'");
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

		*(opt.out)<< player1->first << " to beat " << player2->first
		          << " (" << (e*100) << "%)" << endl;
}

int main(int iargs, const char ** args) {
	// Get options from command line
	opt_t opt = read_options(iargs,args);

	bool have_results = opt.in_fmt != opt_t::IN_ELO;
	bool need_elo = opt.out_fmt != opt_t::OUT_LIST;

	results_t results;
	score_t scores;

	if(have_results){
		// Generate a list of match outcomes from input
		try{
			results = get_results(opt);
		}
		catch (std::exception& e){
			cerr << "Parsing error! Error was:" << endl
				 << e.what() << endl;
			return 1;
		}

		//Use correct prediction method
		prediction_method* predictor = nullptr;
		if(opt.out_fmt != opt_t::OUT_LIST){
			switch(opt.method){
				case opt_t::ELO_METHOD:
					predictor = new elo(opt.cor);
			}
		}

		// Calculate Elo Ratings
		if(need_elo && predictor != nullptr){
			scores = (*predictor)(results);
		}
	}


	// Output in relevant format
	switch(opt.out_fmt){
	case opt_t::OUT_ELO:
		for(auto k:scores){
			*(opt.out) << k.second << "\t" << k.first << endl;
		}
		break;
	case opt_t::OUT_LIST:
		for(auto& k:results){
			*(opt.out) << k.name1  << " " << k.score1 <<" - "
			           << k.score2 << " " << k.name2  << endl;
		}
		break;
	case opt_t::OUT_COMP:
		try{
			do_comparison(opt,scores);
		}
		catch (std::exception& e){
			cerr << "Could not perform player comparison. Error was: "<< endl
			     << e.what() << endl;
			return 1;
		}
		break;
	default:
		throw runtime_error("Unimplemented Output Format.");
	}
	return 0;
}
