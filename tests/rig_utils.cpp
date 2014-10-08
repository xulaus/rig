#include <random>
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

TEST_CASE("Expected outcome behaves sensibly","[expected_outcome]"){
	SECTION("Higher Elo score wins"){
		REQUIRE(expected_outcome(1024,1000) > 0.5);
		REQUIRE(expected_outcome(987,1987) < 0.5);
	}
}
TEST_CASE("Matching names works as a equality operator","[match_name]"){
	SECTION("Identical names return true"){
		REQUIRE(match_name("Player one","Player one")==true);
	}
	SECTION("Dissimilar name return false "){
		REQUIRE(match_name("Player one","Player on")==false);
	}
}

TEST_CASE("Matching abbreviated name works.","[match_name]"){
	SECTION("Abbreviated first names expand"){
		REQUIRE(match_name("P. one","Player one")==true);
	}
}
