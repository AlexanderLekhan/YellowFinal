#include <sstream>
#include <algorithm>

#include "database.h"
#include "flag.h"
#include "test_runner.h"

using namespace std;

//---------------------------------------------------------------------------//
void Database::Add(const Date& date, const string& event)
{
    if (event.empty())
        throw logic_error("E");
        //g_log << 'E';
        //return;
    /*
    size_t p = event.find_first_not_of(" \t\v\n\r");
    string e = (p != string::npos && p > 0)?
                event.substr(p) :
                event;
    */

    //cerr << "Database::Add(" << date << ", " << event << ")" << endl;

    DayEvents& dayEvents = m_storage[date];

    if (dayEvents.m_set.count(event) == 0)
    {
        dayEvents.m_list.push_back(dayEvents.m_set.insert(event).first);
    }
}

//---------------------------------------------------------------------------//
string Database::Last(const Date& date) const
{
    //cerr << "Database::Last(" << date << ")" << endl;
    //cerr << 'L';

    auto it = m_storage.end();

    if (date.GetYear() == 0
        && date.GetMonth() == 0
        && date.GetDay() == 0
        && m_storage.size() > 0)
    {
        //g_log << 'L';
        throw logic_error("L");
    }
    else
    {
        it = m_storage.upper_bound(date);
    }

    if (it == m_storage.begin())
    {
        //g_flag = true;
        //g_log << 'L';
        throw invalid_argument("No entries");
        //return string("");
        //return string("No entries");
    }

    --it;
    ostringstream os;
    PrintEvent(os, it->first, *it->second.m_list.back());
    return os.str();
}

//---------------------------------------------------------------------------//
void Database::Print(ostream& os) const
{
    //cerr << "Database::Print()" << endl;
    //cerr << 'P';

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
    //cerr << "Database::FindIf()" << endl;
    //cerr << 'F';

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
    //cerr << "Database::RemoveIf()" << endl;
    //cerr << 'R';

    int removedTotal = 0;
    vector<Date> daysToRemove;

    for (auto& day : m_storage)
    {
        ASSERT_EQ(day.second.m_list.size(), day.second.m_set.size());

        const Date& date = day.first;
        auto& dayEventList = day.second.m_list;
        auto& dayEventSet = day.second.m_set;

        auto removedIt =
            //remove_if(
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
            //size_t newSize = distance(dayEventList.begin(), removedIt);

            for (auto it = removedIt; it != dayEventList.end(); ++it)
            {
                dayEventSet.erase(*it);
            }
            dayEventList.erase(removedIt, dayEventList.end());

#if 0
            while (dayEventList.size() > newSize)
            {
                dayEventSet.erase(dayEventList.back());
                dayEventList.pop_back();
            }
#endif

#if 0
            for (auto it = removedIt; it != dayEventList.end(); ++it)
            {
                day.second.m_set.erase(*it);
            }
            dayEventList.resize(newSize);
#endif

#if 0
            for (auto it = removedIt; it != dayEventList.end(); ++it)
            {
                //StringSet::const_iterator eventIt = *it;
                //*it = day.second.m_set.end();
                //day.second.m_set.erase(eventIt);
                day.second.m_set.erase(*it);
            }
            //dayEventList.erase(removedIt, dayEventList.end());
#endif
        }
        else
        {
            daysToRemove.push_back(date);
        }
    }
    for (const Date& date : daysToRemove)
    {
        //g_flag = true;
        m_storage.erase(date);
        g_log << 'R';
    }

    return removedTotal;
}