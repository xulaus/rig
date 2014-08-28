#include <random>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "rig_utils.h"

using namespace std;

void cor_tests(double (*cor)(double, double, int,int)){

	random_device rd;
	mt19937 gen(rd());
	normal_distribution<double> elo_dist(1400,2000/7.0);
	uniform_int_distribution<int> result_dist(0,5);

	double delta = 1e-10;

	SECTION("Corrections must be symmetric."){
		for(int i=0;i<100;i++){
			double elo1=elo_dist(gen), elo2=elo_dist(gen);
			int r1=result_dist(gen), r2=5-r1;
			REQUIRE(cor(elo1,elo2,r1,r2) < (-cor(elo2,elo1,r2,r1) + delta));
			REQUIRE((cor(elo1,elo2,r1,r2) + delta) > -cor(elo2,elo1,r2,r1));
		}
	}

}

TEST_CASE("Linear Correction","[linear_correction]"){
	cor_tests(&linear_correction);
}

TEST_CASE("Binomial Correction","[binomial_correction]"){
	cor_tests(binomial_correction);
}

TEST_CASE("Reading reading only one result from tree data","[parse_tree]"){
	stringstream one_match;
	one_match<<"Player one\n"
	         <<"Player two\n"
	         <<"Player one\n"
	         <<"2-6 6-7 5-3 4-1 6-1\n";
	auto results = parse_tree(&one_match);
	SECTION("Only one result should be returned"){
		REQUIRE(results.size() == 1 );
	}
	SECTION("Correct outcome should be returned"){
		outcome_t o = results.at(0);
		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}
		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player two");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==2);
	}
}
