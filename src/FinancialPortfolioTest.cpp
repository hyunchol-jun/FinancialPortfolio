#include "FinancialPortfolio.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

class AFinancialPortfolio: public Test
{
protected:
    static const std::string IBM;
    FinancialPortfolio portfolio;
};
const std::string AFinancialPortfolio::IBM{"IBM"};

TEST_F(AFinancialPortfolio, IsEmptyWhenCreated)
{
    ASSERT_TRUE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, IsNotEmptyAfterPurchase)
{
    portfolio.purchase(IBM, 1);

    ASSERT_FALSE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol)
{
    ASSERT_THAT(portfolio.shareCount("AAPL"), Eq(0));
}

TEST_F(AFinancialPortfolio, AnswersShareCountForPurchasedSymbol)
{
    portfolio.purchase(IBM, 2);
    ASSERT_THAT(portfolio.shareCount(IBM), Eq(2));
}

TEST_F(AFinancialPortfolio, ThrowsOnPurchaseOfZeroShares)
{
    ASSERT_THROW(portfolio.purchase(IBM, 0), InvalidPurchaseException);
}

TEST_F(AFinancialPortfolio, AnswersShareCountForAppropriateSymbol)
{
    portfolio.purchase(IBM, 1);
    portfolio.purchase(IBM, 2);
    
    ASSERT_THAT(portfolio.shareCount(IBM), Eq(3));
}

