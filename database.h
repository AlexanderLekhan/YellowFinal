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
class Entry
{
public:
    Date date;
    std::string event;

    std::string toString() const;

    static void PrintEvent(std::ostream& os,
                           const Date& date,
                           const std::string& event)
    {
        os << date << ' ' << event;
    }
};

std::ostream& operator << (std::ostream& os, const Entry& e);
bool operator == (const Entry& lhs, const Entry& rhs);

//===========================================================================//
class Database
{
public:
    void Add(const Date& date, const std::string& event);
    Entry Last(const Date &date) const;
    void Print(std::ostream& os) const;
    std::vector<Entry> FindIf(const PredicateType& predicate) const;
    int RemoveIf(const PredicateType& predicate);

private:

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
