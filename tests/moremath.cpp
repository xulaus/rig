#include "catch.hpp"
#include "moremath.h"

TEST_CASE("Choose function behaves sensibly","[choose]"){
	SECTION("Checking behaviour for trivial results"){
		REQUIRE(choose(1,-1) == 0);
		REQUIRE(choose(1, 0) == 1);
		REQUIRE(choose(1, 1) == 1);
		REQUIRE(choose(1, 2) == 0);

		REQUIRE(choose(2,-1) == 0);
		REQUIRE(choose(2, 0) == 1);
		REQUIRE(choose(2, 1) == 2);
		REQUIRE(choose(2, 2) == 1);
		REQUIRE(choose(2, 3) == 0);

		REQUIRE(choose(3,-1) == 0);
		REQUIRE(choose(3, 0) == 1);
		REQUIRE(choose(3, 1) == 3);
		REQUIRE(choose(3, 2) == 3);
		REQUIRE(choose(3, 3) == 1);
		REQUIRE(choose(3, 4) == 0);
	}

	SECTION("Checking behaviour with larger numbers"){
		REQUIRE(choose(1000,   -1) ==    0);
		REQUIRE(choose(1000,    0) ==    1);
		REQUIRE(choose(1000,    1) == 1000);
		REQUIRE(choose(1000,  999) == 1000);
		REQUIRE(choose(1000, 1000) ==    1);
		REQUIRE(choose(1000, 1001) ==    0);
	}
}
