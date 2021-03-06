#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE ("Test Extension: NextP/NextPT- test bip source") {
  auto ast = Simple::SimpleInterface::getAstFromFile("tests/BipTest.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test basic NextP 1->2") {
    auto querystr = std::string("Select BOOLEAN such that NextP(1, 2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test Bill->Mary NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(2, 6)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test Mary->ReturnBill NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(8, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test impossible direct NextP 1->6") {
    auto querystr = std::string("Select BOOLEAN such that NextP(1, 6)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }

  SECTION ("Test Bill->John NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(4, 9)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test John->ReturnBill ifblock NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(10, 5)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test John->ReturnBill elseblock NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(11, 5)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test transitive NextP* 1->6") {
    auto querystr = std::string("Select BOOLEAN such that NextP*(1, 6)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test everything that is after 1: NextP* 1->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(1, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "2", "3", "4", "5", "6", "7",
                                     "8", "9"});
  }

  SECTION ("Test everything that is after 2: NextP* 2->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(2, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"10", "11", "3",
                                                              "4", "5", "6",
                                                              "7", "8", "9"});
  }

  SECTION ("Test everything that is after 3: NextP* 3->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(3, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "8", "9"});
  }

  SECTION ("Test everything that is after 4: NextP* 4->?") {
    auto querystr =
        std::string("prog_line n; Select n such that NextP*(4,   n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "8", "9"});
  }

  SECTION ("Test everything that is after 5: NextP* 5->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(5, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Test everything that is after 6: NextP* 6->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(6, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "7", "8", "9"});
  }

  SECTION ("Test everything that is after 7: NextP* 7->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(7, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "8", "9"});
  }

  SECTION ("Test everything that is after 8: NextP* 8->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(8, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "8", "9"});
  }

  SECTION ("Test everything that is after 9: NextP* 9->?") {
    auto querystr = std::string("prog_line n; Select n such that NextP*(9, n)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "3", "4", "5", "8", "9"});
  }

  delete pkb;
  delete qm;
}

TEST_CASE ("Test Extension: AffectsP/AffectsPT- test bip source") {
  auto ast = Simple::SimpleInterface::getAstFromFile("tests/BipTest.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test AffectsP 1->3") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(1, 6)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test AffectsP 1->10") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(1, 10)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test AffectsP 1->11") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(1, 11)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }

  SECTION ("Test AffectsP 6->8") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(6, 8)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test AffectsP* 1->8") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP*(1, 8)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test AffectsP* 1->5") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP*(1, 5)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  delete pkb;
  delete qm;
}

TEST_CASE ("Test Extension: other query types - test bip source") {
  auto ast = Simple::SimpleInterface::getAstFromFile("tests/BipTest.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test all stmts affected by 1") {
    auto querystr = std::string("assign a; Select a such that AffectsP(1, a)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "3", "5", "6", "8"});
  }

  SECTION ("Test all stmts affected* by 1") {
    auto querystr = std::string("assign a; Select a such that AffectsP*(1, a)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "3", "5", "6", "8"});
  }

  SECTION ("Test that some stmts were affected") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(_, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that some stmts were affected*") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP*(_, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that 1 affects something") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that something affects 10") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP(_, 10)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that 1 affects* something") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP*(1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that something affects* 10") {
    auto querystr = std::string("Select BOOLEAN such that AffectsP*(_, 10)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that some stmts have NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(_, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that some stmts have NextP*") {
    auto querystr = std::string("Select BOOLEAN such that NextP*(_, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that 1 has something NextP") {
    auto querystr = std::string("Select BOOLEAN such that NextP(1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that something is beforeP 10") {
    auto querystr = std::string("Select BOOLEAN such that NextP(_, 10)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that 1 has something NextP*") {
    auto querystr = std::string("Select BOOLEAN such that NextP*(1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test that something is beforeP* 10") {
    auto querystr = std::string("Select BOOLEAN such that NextP*(_, 10)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  delete pkb;
  delete qm;
}
