#ifndef Date_h
#define Date_h

#include <boost/date_time/gregorian/gregorian_types.hpp>

class Date
{
public:
    enum Month
    {
        DummyValue, Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    };

    Date() : m_date(boost::gregorian::day_clock::local_day()) {}

    Date(unsigned int year, Month month, unsigned int day) :
        m_date(year, month, day) {}

    boost::gregorian::date m_date;
};
#endif
