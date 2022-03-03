#include "Holding.h"
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace::testing;
using namespace::boost::gregorian;

class AHolding : public Test
{
protected:
    Holding holding;
    static const date ArbitraryDate;
};
const date AHolding::ArbitraryDate(2014, Sep, 5);

TEST_F(AHolding, ContainsZeroSharesWhenCreated)
{
    ASSERT_THAT(holding.shareCount(), Eq(0));
}

TEST_F(AHolding, AnswersShareCountsAfterAdding)
{
    PurchaseRecord record{5, ArbitraryDate};
    holding.add(record);

    ASSERT_THAT(holding.shareCount(), Eq(5));
}

TEST_F(AHolding, AnswersShareCountsAfterMultipleAdding)
{
    PurchaseRecord record1{5, ArbitraryDate}, record2{10, ArbitraryDate};
    holding.add(record1);
    holding.add(record2);

    ASSERT_THAT(holding.shareCount(), Eq(15));
}
