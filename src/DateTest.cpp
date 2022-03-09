#include "Date.h"
#include <gmock/gmock.h>
#include <boost/date_time/gregorian/gregorian_types.hpp>

using namespace testing;

TEST(ADate, HasBoostDateAsMember)
{
    Date date(2021, Date::Sep, 24);
    boost::gregorian::date boostDate(2021, boost::gregorian::Sep, 24);

    ASSERT_THAT(date.m_date, Eq(boostDate));
}

TEST(ADate, HasTodayAsDateWhenDefaultConstructed)
{
    Date date;
    boost::gregorian::date boostToday(boost::gregorian::day_clock::local_day());

    ASSERT_THAT(date.m_date, Eq(boostToday));
}
