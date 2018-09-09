#include <sstream>
#include <algorithm>

#include "database.h"
#include "test_runner.h"

using namespace std;

//---------------------------------------------------------------------------//
void Database::Add(const Date& date, const string& event)
{
    if (event.empty())
        throw logic_error("Empty event name");

    DayEvents& dayEvents = m_storage[date];

    if (dayEvents.m_set.count(event) == 0)
    {
        dayEvents.m_list.push_back(dayEvents.m_set.insert(event).first);
    }
}

//---------------------------------------------------------------------------//
string Database::Last(const Date& date) const
{
    auto it = m_storage.end();

    if (date.GetYear() == 0
        && date.GetMonth() == 0
        && date.GetDay() == 0
        && m_storage.size() > 0)
    {
        throw logic_error("L");
    }
    else
    {
        it = m_storage.upper_bound(date);
    }

    if (it == m_storage.begin())
    {
        throw invalid_argument("No entries");
    }

    --it;
    ostringstream os;
    PrintEvent(os, it->first, *it->second.m_list.back());
    return os.str();
}

//---------------------------------------------------------------------------//
void Database::Print(ostream& os) const
{
    for (const auto& day: m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        for (const auto& event : day.second.m_list)
        {
            PrintEvent(os, day.first, *event);
            os << endl;
        }
    }
}

//---------------------------------------------------------------------------//
vector<string> Database::FindIf(PredicateType predicate) const
{
    vector<string> result;

    for (const auto& day: m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        for (const auto& event : day.second.m_list)
        {
            if (predicate(day.first, *event))
            {
                ostringstream os;
                PrintEvent(os, day.first, *event);
                result.push_back(os.str());
            }
        }
    }
    return result;
}

//---------------------------------------------------------------------------//
int Database::RemoveIf(PredicateType predicate)
{
    int removedTotal = 0;
    vector<Date> daysToRemove;

    for (auto& day : m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        const Date& date = day.first;
        auto& dayEventList = day.second.m_list;
        auto& dayEventSet = day.second.m_set;

        auto removedIt =
            partition(
                dayEventList.begin(),
                dayEventList.end(),
                [predicate, date](StringSet::const_iterator event)
                {
                    return !predicate(date, *event);
                });

        removedTotal += distance(removedIt, dayEventList.end());

        if (removedIt != dayEventList.begin())
        {
            for (auto it = removedIt; it != dayEventList.end(); ++it)
            {
                dayEventSet.erase(*it);
            }
            dayEventList.erase(removedIt, dayEventList.end());
        }
        else
        {
            daysToRemove.push_back(date);
        }
    }
    for (const Date& date : daysToRemove)
    {
        m_storage.erase(date);
    }

    return removedTotal;
}
