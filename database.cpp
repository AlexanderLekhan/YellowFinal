#include <sstream>
#include <algorithm>

#include "database.h"
#include "test_runner.h"

using namespace std;

//---------------------------------------------------------------------------//
string Entry::toString() const
{
    ostringstream os;
    Entry::PrintEvent(os, date, event);
    return os.str();
}

//---------------------------------------------------------------------------//
ostream& operator << (ostream& os, const Entry& e)
{
    Entry::PrintEvent(os, e.date, e.event);
    return os;
}

//---------------------------------------------------------------------------//
bool operator == (const Entry& lhs, const Entry& rhs)
{
    return tie(lhs.date, lhs.event) == tie(rhs.date, rhs.event);
}

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
Entry Database::Last(const Date& date) const
{
    auto it = m_storage.upper_bound(date);

    if (it == m_storage.begin())
    {
        throw invalid_argument("No entries");
    }

    --it;
    return {it->first, *it->second.m_list.back()};
}

//---------------------------------------------------------------------------//
void Database::Print(ostream& os) const
{
    for (const auto& day: m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        for (const auto& event : day.second.m_list)
        {
            Entry::PrintEvent(os, day.first, *event);
            os << endl;
        }
    }
}

//---------------------------------------------------------------------------//
std::vector<Entry> Database::FindIf(const PredicateType& predicate) const
{
    vector<Entry> result;

    for (const auto& day: m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        for (const auto& event : day.second.m_list)
        {
            if (predicate(day.first, *event))
            {
                result.push_back({day.first, *event});
            }
        }
    }
    return result;
}

//---------------------------------------------------------------------------//
int Database::RemoveIf(const PredicateType& predicate)
{
    int removedTotal = 0;
    vector<Date> daysToRemove;

    for (auto& day : m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        const Date& date = day.first;
        auto& dayEventList = day.second.m_list;
        auto& dayEventSet = day.second.m_set;
        vector<string> eventsToRemove;

        auto removedIt =
            stable_partition(
                dayEventList.begin(),
                dayEventList.end(),
                [&predicate, &date](StringSet::const_iterator event)
                {
                    return !predicate(date, *event);
                });

        removedTotal += distance(removedIt, dayEventList.end());

        if (removedIt != dayEventList.begin())
        {
            for (auto eventIt = removedIt; eventIt != dayEventList.end(); ++eventIt)
            {
                dayEventSet.erase(*eventIt);
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
