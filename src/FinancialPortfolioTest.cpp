#include "FinancialPortfolio.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

using namespace ::testing;

class AFinancialPortfolio: public Test
{
protected:
    static const std::string IBM;
    static const std::string SAMSUNG;
    FinancialPortfolio portfolio;
};
const std::string AFinancialPortfolio::IBM{"IBM"};
const std::string AFinancialPortfolio::SAMSUNG{"SSNLF"};

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
    portfolio.purchase(IBM, 10);
    portfolio.purchase(SAMSUNG, 5);
    
    ASSERT_THAT(portfolio.shareCount(IBM), Eq(10));
}

TEST_F(AFinancialPortfolio, 
        ShareCountReflectsAccumulatedPurchasesOfSameSymbol)
{
    portfolio.purchase(IBM, 5);
    portfolio.purchase(IBM, 3);

    ASSERT_THAT(portfolio.shareCount(IBM), Eq(8));
}

TEST_F(AFinancialPortfolio, ReducesShareCountOfSymbolOnSell)
{
    portfolio.purchase(IBM, 5);
    portfolio.sell(IBM, 3);

    ASSERT_THAT(portfolio.shareCount(IBM), Eq(2));
}

TEST_F(AFinancialPortfolio, ThrowsWhenSellingMoreSharesThanPurchased)
{
    portfolio.purchase(IBM, 5);
    
    ASSERT_THROW(portfolio.sell(IBM, 10), InvalidSellException);
}

TEST_F(AFinancialPortfolio, AnswersThePurchaseRecordForASinglePurchase)
{
    portfolio.purchase(SAMSUNG, 5);
    auto purchases = portfolio.purchases(SAMSUNG);

    auto purchase = purchases[0];
    ASSERT_THAT(purchase.shareCount, Eq(5));
    ASSERT_THAT(purchase.date, Eq(FinancialPortfolio::FIXED_PURCHASE_DATE));
}
