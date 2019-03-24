#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE ("Test Query Executor With functionality - system_tests/src/1.txt") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test basic with: 2 = 2") {
    auto querystr = std::string("assign a; Select a with 2 = 2");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION ("Test basic with: 2 = 3") {
    auto querystr = std::string("assign a; Select a with 2 = 3");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Test basic with: \"asdf\" = \"asdf\"") {
    auto querystr = std::string("assign a; Select a with \"asdf\" = \"asdf\"");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION ("Test basic with:\"asdf\" = \"asdggg\"") {
    auto querystr =
        std::string("assign a; Select a with \"asdf\" = \"asdggg\"");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Test basic with: n1 = n2") {
    auto querystr = std::string("prog_line n1, n2; Select n1 with n1 = n2");
    std::vector<std::string> proglines = {
        "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "2", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "3", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "4", "40", "41", "42", "43", "5",  "6",  "7",  "8",  "9"};
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == proglines);
  }

  SECTION (
      "Test with that has n2 constrained to 1 value: n1 = n2, Next(1, n2)") {
    auto querystr = std::string(
        "prog_line n1, n2; Select n1 such that Next(1, n2) with n1 = n2");
    std::vector<std::string> proglines = {"2"};
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == proglines);
  }

  SECTION ("Test impossible with: n1 = n2, Next(n1, n2)") {
    auto querystr = std::string(
        "prog_line n1, n2; Select n1 such that Next(n1, n2) with n1 = n2");
    std::vector<std::string> proglines = {};
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == proglines);
  }

  SECTION (
      "Test lines that have control flow to themselves with: n1 = n2, "
      "Next*(n1, n2)") {
    auto querystr = std::string(
        "prog_line n1, n2; Select n1 such that Next*(n1, n2) with n1 = n2");
    std::vector<std::string> proglines = {"10", "11", "12", "13",
                                          "14", "15", "16", "9"};
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == proglines);
  }

  SECTION ("Test basic with: n1 = n1") {
    auto querystr = std::string("prog_line n1, n2; Select n1 with n1 = n1");
    std::vector<std::string> proglines = {
        "1", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "2", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "3", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "4", "40", "41", "42", "43", "5",  "6",  "7",  "8",  "9"};
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == proglines);
  }

  delete pkb;
  delete qm;
}
