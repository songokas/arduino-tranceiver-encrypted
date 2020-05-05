#include "fakeit.hpp"
#include "catch.hpp"

#include <string.h>

#include "CommonModule/StringHelper.h"

using namespace fakeit;
using CommonModule::findNextPos;
using CommonModule::findPosFromEnd;

TEST_CASE( "helper functions", "[helpers]" ) {

    const char * str1 = "test1/test2.";

    SECTION( "findNextPos" ) {
        REQUIRE(findNextPos(str1, strlen(str1), '/') == 6);
        REQUIRE(findNextPos(str1, strlen(str1), '/', 2) == 0);
        REQUIRE(findNextPos(str1, strlen(str1), '1') == 5);
        REQUIRE(findNextPos(str1, strlen(str1), '2') == 11);
        REQUIRE(findNextPos(str1, strlen(str1), '.') == 0);
        REQUIRE(findNextPos(str1, 5, '1') == 0);
        REQUIRE(findNextPos(str1, 6, '1') == 5);
        REQUIRE(findNextPos(str1, 100, '1') == 0);
    }

    SECTION( "findPosFromEnd" ) {
        REQUIRE(findPosFromEnd(str1, strlen(str1), '/') == 5);
        REQUIRE(findPosFromEnd(str1, strlen(str1), '1') == 4);
        REQUIRE(findPosFromEnd(str1, strlen(str1), '2') == 10);
        REQUIRE(findPosFromEnd(str1, strlen(str1), 't') == 9);
        REQUIRE(findPosFromEnd(str1, 8, 't') == 6);
        REQUIRE(findPosFromEnd(str1, 100, '1') == 0);
    }
}
