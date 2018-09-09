#include <stdexcept>
#include <iomanip>
#include <vector>

#include "date.h"

using namespace std;

//===========================================================================//
void wrong_date_format_error()
{
    throw invalid_argument("Wrong date format");
}

bool skip_date_delim(istream& in)
{
    if (!(in && in.peek() == '-'))
        wrong_date_format_error();

    in.ignore();
    return true;
}

//===========================================================================//
Date::Date(int year, int month, int day)
{
    if (year < 0 || year > 9999)
        throw logic_error("Year value is invalid: " + to_string(year));

    m_year = year;

    if (month < 1 || month > 12)
        throw logic_error("Month value is invalid: " + to_string(month));

    m_month = month;

    if (day < 1 || day > 31)
        throw logic_error("Day value is invalid: " + to_string(day));

    m_day = day;
}

//---------------------------------------------------------------------------//
bool operator<(const Date& lhs, const Date& rhs)
{
    return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
        vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
    /*
    if (lhs.GetYear() != rhs.GetYear())
    {
        return lhs.GetYear() < rhs.GetYear();
    }
    if (lhs.GetMonth() != rhs.GetMonth())
    {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetDay() < rhs.GetDay();
    */
}

//---------------------------------------------------------------------------//
ostream& operator<<(ostream& out, const Date& d)
{
    return out << setfill('0')
               << setw(4) << d.GetYear() << '-'
               << setw(2) << d.GetMonth() << '-'
               << setw(2) << d.GetDay();
}

//---------------------------------------------------------------------------//
Date ParseDate(istream& is)
{
    int year = 0;
    int month = 0;
    int day = 0;

    is >> year;
    skip_date_delim(is);
    is >> month;
    skip_date_delim(is);
    is >> day;

    return Date(year, month, day);
}
