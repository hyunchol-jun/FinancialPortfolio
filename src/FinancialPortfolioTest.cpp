#include "FinancialPortfolio.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

class AFinancialPortfolio: public Test
{
protected:
    FinancialPortfolio portfolio;
};

TEST_F(AFinancialPortfolio, IsEmptyWhenCreated)
{
    ASSERT_TRUE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, IsNotEmptyAfterPurchase)
{
    portfolio.purchase("IBM", 1);

    ASSERT_FALSE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol)
{
    ASSERT_THAT(portfolio.shareCount("AAPL"), Eq(0));
}
