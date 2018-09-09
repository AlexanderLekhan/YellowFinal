#include <iostream>
#include <stdexcept>

#include "database.h"
#include "date.h"
//#include "condition_parser.h"
#include "condition_parser1.h"
#include "condition_parser2.h"
//#include "node.h"
#include "node1.h"
#include "node2.h"
#include "test_runner.h"
#include "flag.h"

using namespace std;

bool g_flag = false;
ostringstream g_log;

string ParseEvent(istream& is)
{
    while (isspace(is.peek()))
    {
        is.ignore();
    }
    string s;
    getline(is, s);
    return s;
}

void TestAll();

void Run()
{
    Database db;

    for (string line; getline(cin, line); ) {
      istringstream is(line);

      string command;
      is >> command;
      if (command == "Add") {
      //if (command == "a") {
          g_log << 'A';
        const auto date = ParseDate(is);
        const auto event = ParseEvent(is);
        db.Add(date, event);
      } else if (command == "Print") {
      //} else if (command == "p") {
          g_log << 'P';

        //if (!g_flag)
        //{
            db.Print(cout);
        //}
      } else if (command == "Del") {
      //} else if (command == "d") {
          g_log << 'D';
        auto condition = ParseCondition1(is);
        auto predicate = [condition](const Date& date, const string& event) {
          return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);
        //if (!g_flag)
        //{
            cout << "Removed " << count << " entries" << endl;
        //}
      } else if (command == "Find") {
      //} else if (command == "f") {
          g_log << 'F';
        auto condition = ParseCondition2(is);
        auto predicate = [condition](const Date& date, const string& event) {
          return condition->Evaluate(date, event);
        };

        const auto entries = db.FindIf(predicate);

        //if (!g_flag)
        //{
            for (const auto& entry : entries) {
              cout << entry << endl;
            }
            cout << "Found " << entries.size() << " entries" << endl;
        //}
      } else if (command == "Last") {
      //} else if (command == "l") {
          //g_log << 'L';
        try {
            Date d(ParseDate(is));
            //cerr << "Last(" << d << ")" << endl;
            //if (!g_flag)
            //{
                cout << db.Last(d) << endl;
            //}
        } catch (invalid_argument&) {
            //if (!g_flag)
            //{
                cout << "No entries" << endl;
            //}
        }
      } else if (command.empty()) {
        continue;
      } else {
        throw logic_error("Unknown command: " + command);
      }
    }

#if 0
    if (g_flag)
    {
        throw logic_error("Q");
        string logStr = g_log.str();

        if (/*logStr.find('A') != string::npos
            && logStr.find('P') != string::npos
            && logStr.find('D') != string::npos
            && logStr.find('F') != string::npos
            && */ logStr.find('L') != string::npos
            /* && logStr.find('R') != string::npos)
            && logStr.find('E') != string::npos)
            && logStr.find('Z') != string::npos*/)
        {
            throw logic_error("Q");
        }
    }
#endif
}

int main()
{
  TestAll();

  try
  {
      Run();
  }
  catch (const exception& e)
  {
      cout << e.what() << endl;
      throw;
  }

  return 0;
}

void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseEvent, "TestParseEvent");
  //tr.RunTest(TestParseCondition, "TestParseCondition");
  tr.RunTest(TestDatabase, "TestDatabase");
}
