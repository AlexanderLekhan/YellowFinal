#include "condition_parser.h"
#include "database.h"
#include "test_runner.h"

using namespace std;

void TestFind(Database& db,
              const string& condition,
              const vector<string>& test)
{
    istringstream is(condition);
    auto conditionFind = ParseCondition(is);
    auto predicate = [conditionFind](const Date& date, const string& event) {
      return conditionFind->Evaluate(date, event);
    };
    ASSERT_EQ(db.FindIf(predicate), test);
}

void TestRemove(Database& db,
                const string& condition,
                int numRemoved,
                const vector<string>& test)
{
    istringstream isRemove(condition);
    auto conditionRemove = ParseCondition(isRemove);
    auto predicateRemove = [conditionRemove](const Date& date, const string& event) {
      return conditionRemove->Evaluate(date, event);
    };

    ASSERT_EQ(db.RemoveIf(predicateRemove), numRemoved);

    istringstream isFind("");
    auto conditionFind = ParseCondition(isFind);
    auto predicateFind = [conditionFind](const Date& date, const string& event) {
      return conditionFind->Evaluate(date, event);
    };
    ASSERT_EQ(db.FindIf(predicateFind), test);
}

//---------------------------------------------------------------------------//
void TestDatabase()
{
    Database db;

    /////////////////// Last ///////////////////////
    // Empty
    try
    {
        ASSERT_EQ(db.Last({1, 1, 1}), "No entries");
    }
    catch (const invalid_argument& e)
    {
        ASSERT_EQ(e.what(), string("No entries"));
    }

    // 1 entry
    db.Add({2, 2, 2}, "k");

    // Before all
    try
    {
        ASSERT_EQ(db.Last({1, 1, 1}), "No entries");
    }
    catch (const invalid_argument& e)
    {
        ASSERT_EQ(e.what(), string("No entries"));
    }
    // Exact
    ASSERT_EQ(db.Last({2, 2, 2}), "0002-02-02 k");
    // After
    ASSERT_EQ(db.Last({2, 2, 3}), "0002-02-02 k");
    ASSERT_EQ(db.Last({2, 3, 3}), "0002-02-02 k");
    ASSERT_EQ(db.Last({3, 3, 3}), "0002-02-02 k");

    // Order of 2 entries in 1 date
    db.Add({2, 2, 2}, "e");
    // Exact
    ASSERT_EQ(db.Last({2, 2, 2}), "0002-02-02 e");
    // After
    ASSERT_EQ(db.Last({2, 2, 3}), "0002-02-02 e");
    ASSERT_EQ(db.Last({2, 3, 3}), "0002-02-02 e");
    ASSERT_EQ(db.Last({3, 3, 3}), "0002-02-02 e");

    // 2 dates
    db.Add({3, 3, 3}, "q");
    // Exact first
    ASSERT_EQ(db.Last({2, 2, 2}), "0002-02-02 e");
    // After first
    ASSERT_EQ(db.Last({2, 2, 3}), "0002-02-02 e");
    ASSERT_EQ(db.Last({2, 3, 3}), "0002-02-02 e");
    // Exact second
    ASSERT_EQ(db.Last({3, 3, 3}), "0003-03-03 q");
    // After second
    ASSERT_EQ(db.Last({3, 3, 4}), "0003-03-03 q");
    ASSERT_EQ(db.Last({3, 4, 4}), "0003-03-03 q");

    // 3 dates
    db.Add({4, 4, 4}, "i");
    // Exact first
    ASSERT_EQ(db.Last({2, 2, 2}), "0002-02-02 e");
    // After first
    ASSERT_EQ(db.Last({2, 2, 3}), "0002-02-02 e");
    ASSERT_EQ(db.Last({2, 3, 3}), "0002-02-02 e");
    // Exact second
    ASSERT_EQ(db.Last({3, 3, 3}), "0003-03-03 q");
    // After second
    ASSERT_EQ(db.Last({3, 3, 4}), "0003-03-03 q");
    ASSERT_EQ(db.Last({3, 4, 4}), "0003-03-03 q");
    // Exact third
    ASSERT_EQ(db.Last({4, 4, 4}), "0004-04-04 i");
    // After third
    ASSERT_EQ(db.Last({4, 4, 5}), "0004-04-04 i");
    ASSERT_EQ(db.Last({4, 5, 5}), "0004-04-04 i");


    /////////////////// Add existing ///////////////////////
    db.Add({2, 2, 2}, "k");
    db.Add({3, 3, 3}, "e");

    /////////////////// FindIf ///////////////////////
    TestFind(db, "",
        {
            "0002-02-02 k",
            "0002-02-02 e",
            "0003-03-03 q",
            "0003-03-03 e",
            "0004-04-04 i"
        });

    TestFind(db, R"c(event == "e" AND event != "e")c",
        {
        });

    TestFind(db, R"c(event == "e" OR event != "e")c",
        {
            "0002-02-02 k",
            "0002-02-02 e",
            "0003-03-03 q",
            "0003-03-03 e",
            "0004-04-04 i"
        });

    TestFind(db, R"c(event > "f" AND event < "m")c",
        {
            "0002-02-02 k",
            "0004-04-04 i"
        });

    /////////////////// RemoveIf ///////////////////////
    TestRemove(db, R"c(event == "e")c", 2,
        {
           "0002-02-02 k",
           "0003-03-03 q",
           "0004-04-04 i"
        });

    db.Add({2, 2, 2}, "e");
    db.Add({3, 3, 3}, "e");

    TestRemove(db, R"c(event == "e" OR date >= 3-3-3)c", 4,
        {
           "0002-02-02 k"
        });

    TestRemove(db, "", 1, {});
}
