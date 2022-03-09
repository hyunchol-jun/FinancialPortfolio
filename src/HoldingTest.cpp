#include "Holding.h"
#include "Date.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace::testing;

class AHolding : public Test
{
protected:
    Holding holding;
    static const Date ArbitraryDate;
};
const Date AHolding::ArbitraryDate(2014, Date::Sep, 5);

TEST_F(AHolding, ContainsZeroSharesWhenCreated)
{
    ASSERT_THAT(holding.shareCount(), Eq(0));
}

TEST_F(AHolding, AnswersShareCountsAfterAdding)
{
    PurchaseRecord record{5, 100.0, ArbitraryDate};
    holding.add(record);

    ASSERT_THAT(holding.shareCount(), Eq(5));
}

TEST_F(AHolding, AnswersShareCountsAfterMultipleAdding)
{
    PurchaseRecord record1{5, 100.0, ArbitraryDate};
    PurchaseRecord record2{10, 200.0, ArbitraryDate};
    holding.add(record1);
    holding.add(record2);

    ASSERT_THAT(holding.shareCount(), Eq(15));
}
