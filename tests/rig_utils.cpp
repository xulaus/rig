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

TEST_CASE("Reading one result from list data.","[parse_list]"){
	stringstream one_match;
	one_match<<"Player one 3- 2 Player two";
	auto results = parse_list(&one_match);
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

TEST_CASE("Reading multiple results from list data.","[parse_list]"){
	stringstream mul_match;
	mul_match<<"Player two 2 -3 Player one\n"
	         <<"Player three 1-0 Player four\n"
	         <<"Player three  0  -  3  Player one\n";
	auto results = parse_list(&mul_match);
	SECTION("Three results should be returned"){
		REQUIRE(results.size() == 3 );
	}

	SECTION("Correct outcome should be returned"){
		int index=0;
		outcome_t o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player two");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==2);

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player three");
		REQUIRE(o.name2=="Player four");
		REQUIRE(o.score1==1);
		REQUIRE(o.score2==0);

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player three");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==0);
	}
}
TEST_CASE("Reading result from tree data abbreviated name","[parse_tree]"){
	stringstream one_match;
	one_match<<"Player one\n"
	         <<"Player two\n"
	         <<"P. one\n"
	         <<"6/3 7/5 6/4\n";
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
		REQUIRE(o.score2==0);
	}
}
TEST_CASE("Reading result from tree data with space in score","[parse_tree]"){
	stringstream one_match;
	one_match<<"Player one\n"
	         <<"Player two\n"
	         <<"Player one\n"
	         <<"3  /  5 6 / 3 7/ 5 6 /4\n";
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
		REQUIRE(o.score2==1);
	}
}

TEST_CASE("Reading only one result from tree data","[parse_tree]"){
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

TEST_CASE("Reading multiple results from tree data","[parse_tree]"){
	stringstream mul_match;
	mul_match<<"Player two\n"
	         <<"Player one\n"
	         <<"Player three\n"
	         <<"Player four\n"
	         <<"Player one\n"
	         <<"2-6 5-1 7-5 5-7 6-0\n"
	         <<"Player three\n"
	         <<"6-1\n"
	         <<"Player one\n"
	         <<"6-5 6-3 3-1\n";
	auto results = parse_tree(&mul_match);
	SECTION("Three results should be returned"){
		REQUIRE(results.size() == 3 );
	}

	SECTION("Correct outcome should be returned"){
		int index=0;
		outcome_t o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player two");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==2);

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player three");
		REQUIRE(o.name2=="Player four");
		REQUIRE(o.score1==1);
		REQUIRE(o.score2==0);

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player three");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==0);
	}
}

TEST_CASE("Reading single walkover result from tree data","[parse_tree]"){
	stringstream one_match;
	one_match<<"Player one\n"
	         <<"Player two\n"
	         <<"Player one\n"
	         <<"wo\n";
	auto results = parse_tree(&one_match);

	SECTION("Correct outcome should be returned"){
		if( results.size() == 1 ){
			outcome_t o = results.at(0);
			if(o.score1 < o.score2){
				swap(o.name1,o.name2);
				swap(o.score1,o.score2);
			}
			REQUIRE(o.name1=="Player one");
			REQUIRE(o.name2=="Player two");
			REQUIRE(o.score1==0);
			REQUIRE(o.score2==0);
		}
		else{
			REQUIRE(results.size()==0);
		}
	}

}

TEST_CASE("Reading multiple results including walkover result and irregular match lengths from tree data","[parse_tree]"){
	stringstream mul_match;
	mul_match<<"Player one\n"
	         <<"Player two\n"
	         <<"Player three\n"
	         <<"Player four\n"
	         <<"Player one\n"
	         <<"wo\n"
	         <<"Player three\n"
	         <<"6-1\n"
	         <<"Player one\n"
	         <<"6-5 6-3 3-1\n";
	auto results = parse_tree(&mul_match);
	SECTION("Three results should be returned"){
		REQUIRE(results.size() == 3 );
	}

	SECTION("Correct outcome should be returned"){
		int index=0;
		outcome_t o = results.at(index);

		if(o.name1=="Player one"){
			REQUIRE(o.name1=="Player one");
			REQUIRE(o.name2=="Player two");
			REQUIRE(o.score1==0);
			REQUIRE(o.score2==0);
			index++;
		}

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player three");
		REQUIRE(o.name2=="Player four");
		REQUIRE(o.score1==1);
		REQUIRE(o.score2==0);

		o = results.at(index++);

		if(o.score1 < o.score2){
			swap(o.name1,o.name2);
			swap(o.score1,o.score2);
		}

		REQUIRE(o.name1=="Player one");
		REQUIRE(o.name2=="Player three");
		REQUIRE(o.score1==3);
		REQUIRE(o.score2==0);
	}
}
