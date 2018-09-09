#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date
{
public:
    Date(int year, int month, int day);

    int GetYear() const
    {
        return m_year;
    }
    int GetMonth() const
    {
        return m_month;
    }
    int GetDay() const
    {
        return m_day;
    }

private:
    int m_year = 0;
    int m_month = 1;
    int m_day = 1;
};

bool operator<(const Date& lhs, const Date& rhs);

std::ostream& operator<<(std::ostream& out, const Date& d);

Date ParseDate(std::istream& is);

#endif // DATE_H
