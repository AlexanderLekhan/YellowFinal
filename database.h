#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>
#include <functional>

#include "date.h"

//===========================================================================//
using PredicateType =
        std::function<bool(const Date& date, const std::string& event)>;

//===========================================================================//
class Database
{
public:
    void Add(const Date& date, const std::string& event);
    std::string Last(const Date& date) const;
    void Print(std::ostream& os) const;
    std::vector<std::string> FindIf(PredicateType predicate) const;
    int RemoveIf(PredicateType predicate);

private:

    static void PrintEvent(std::ostream& os,
                           const Date& date,
                           const std::string& event)
    {
        os << date << ' ' << event;
    }

    using StringSet = std::set<std::string>;

    struct DayEvents
    {
        StringSet m_set;
        std::vector<StringSet::const_iterator> m_list;
    };
    std::map<Date, DayEvents> m_storage;
};

void TestDatabase();

#endif // DATABASE_H
