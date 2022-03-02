#include "FinancialPortfolio.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <iostream>

using namespace ::testing;
using namespace boost::gregorian;

class AFinancialPortfolio: public Test
{
protected:
    static const std::string IBM;
    static const std::string SAMSUNG;
    FinancialPortfolio portfolio;
    static const date ArbitraryDate;

    void purchase(
            const std::string& ticker,
            int shareCount,
            const date& 
                transactionDate=AFinancialPortfolio::ArbitraryDate)
    {
        portfolio.purchase(ticker, shareCount, transactionDate);
    }

    void sell(
            const std::string& ticker,
            int shareCount,
            const date&
                transactionDate=AFinancialPortfolio::ArbitraryDate)
    {
        portfolio.sell(ticker, shareCount, transactionDate);
    }

    void ASSERT_PURCHASE(
            PurchaseRecord& purchase, int shareCount, const date& date)
    {
        ASSERT_THAT(purchase.shareCount, Eq(shareCount));
        ASSERT_THAT(purchase.date, Eq(date));
    }
};

const date AFinancialPortfolio::ArbitraryDate(2014, Sep, 5);
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
    ASSERT_THROW(portfolio.purchase(IBM, 0), ShareCountCannotBeZeroException);
}

TEST_F(AFinancialPortfolio, ThrowsOnSellOfZeroShares)
{
    ASSERT_THROW(portfolio.sell(IBM, 0), ShareCountCannotBeZeroException);
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
    purchase(IBM, 5);
    portfolio.sell(IBM, 3);

    ASSERT_THAT(portfolio.shareCount(IBM), Eq(2));
}

TEST_F(AFinancialPortfolio, ThrowsWhenSellingMoreSharesThanPurchased)
{
    portfolio.purchase(IBM, 5);
    
    ASSERT_THROW(portfolio.sell(IBM, 10), InsufficientSharesException);
}

TEST_F(AFinancialPortfolio, AnswersThePurchaseRecordForASinglePurchase)
{
    purchase(SAMSUNG, 5, ArbitraryDate);

    auto purchases = portfolio.purchases(SAMSUNG);

    ASSERT_PURCHASE(purchases[0], 5, ArbitraryDate);
}

TEST_F(AFinancialPortfolio, IncludesSalesInPurchaseRecords)
{
    purchase(SAMSUNG, 10);
    sell(SAMSUNG, 5, ArbitraryDate);
    
    auto sales = portfolio.purchases(SAMSUNG);

    ASSERT_PURCHASE(sales[1], -5, ArbitraryDate);
}

bool operator==(const PurchaseRecord& lhs, const PurchaseRecord& rhs)
{
    return lhs.shareCount == rhs.shareCount && lhs.date == rhs.date;
}

TEST_F(AFinancialPortfolio, SeparatesPurchaseRecordsByTicker)
{
    purchase(SAMSUNG, 5, ArbitraryDate);
    purchase(IBM, 1, ArbitraryDate);

    auto sales = portfolio.purchases(SAMSUNG);
    ASSERT_THAT(sales, ElementsAre(PurchaseRecord(5, ArbitraryDate)));
}

TEST_F(AFinancialPortfolio, 
        AnswersEmptyPurchaseRecordVectorWhenTickerNotFound)
{
    ASSERT_THAT(portfolio.purchases(SAMSUNG), 
                        Eq(std::vector<PurchaseRecord>()));
}
