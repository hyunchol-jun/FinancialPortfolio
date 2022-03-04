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
    static const date TODAY;

    void purchase(const std::string& ticker,
                  int shareCount,
                  double purchasePrice=0.0,
                  const date& 
                        transactionDate=AFinancialPortfolio::ArbitraryDate)
    {
        portfolio.purchase(ticker, 
                            {shareCount, purchasePrice, transactionDate});
    }

    void sell(const std::string& ticker,
              int shareCount,
              double salePrice=0.0,
              const date&
                        transactionDate=AFinancialPortfolio::ArbitraryDate)

    {
        portfolio.sell(ticker,
                            {shareCount, salePrice, transactionDate});
    }

    void ASSERT_PURCHASE(PurchaseRecord& purchase, 
                         int shareCount, 
                         double purchasePrice,
                         const date& date)
    {
        ASSERT_THAT(purchase.shareCount, Eq(shareCount));
        ASSERT_THAT(purchase.priceOnTransaction, DoubleEq(purchasePrice));
        ASSERT_THAT(purchase.date, Eq(date));
    }
};

const date AFinancialPortfolio::ArbitraryDate(2014, Sep, 5);
const date AFinancialPortfolio::TODAY(day_clock::local_day());
const std::string AFinancialPortfolio::IBM{"IBM"};
const std::string AFinancialPortfolio::SAMSUNG{"SSNLF"};

TEST_F(AFinancialPortfolio, IsEmptyWhenCreated)
{
    ASSERT_TRUE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, IsNotEmptyAfterPurchase)
{
    purchase(IBM, 1);

    ASSERT_FALSE(portfolio.isEmpty());
}

TEST_F(AFinancialPortfolio, AnswersZeroForShareCountOfUnpurchasedSymbol)
{
    ASSERT_THAT(portfolio.shareCount("AAPL"), Eq(0));
}

TEST_F(AFinancialPortfolio, AnswersShareCountForPurchasedSymbol)
{
    purchase(IBM, 2);
    ASSERT_THAT(portfolio.shareCount(IBM), Eq(2));
}

TEST_F(AFinancialPortfolio, ThrowsOnPurchaseOfZeroShares)
{
    ASSERT_THROW(purchase(IBM, 0), ShareCountCannotBeZeroException);
}

TEST_F(AFinancialPortfolio, ThrowsOnSellOfZeroShares)
{
    ASSERT_THROW(sell(IBM, 0), ShareCountCannotBeZeroException);
}

TEST_F(AFinancialPortfolio, AnswersShareCountForAppropriateSymbol)
{
    purchase(IBM, 10);
    purchase(SAMSUNG, 5);
    
    ASSERT_THAT(portfolio.shareCount(IBM), Eq(10));
}

TEST_F(AFinancialPortfolio, 
        ShareCountReflectsAccumulatedPurchasesOfSameSymbol)
{
    purchase(IBM, 5);
    purchase(IBM, 3);

    ASSERT_THAT(portfolio.shareCount(IBM), Eq(8));
}

TEST_F(AFinancialPortfolio, ReducesShareCountOfSymbolOnSell)
{
    purchase(IBM, 5);
    sell(IBM, 3);

    ASSERT_THAT(portfolio.shareCount(IBM), Eq(2));
}

TEST_F(AFinancialPortfolio, ThrowsWhenSellingMoreSharesThanPurchased)
{
    purchase(IBM, 5);
    
    ASSERT_THROW(sell(IBM, 10), InsufficientSharesException);
}

TEST_F(AFinancialPortfolio, AnswersThePurchaseRecordForASinglePurchase)
{
    purchase(SAMSUNG, 5, 100.00, ArbitraryDate);

    auto purchases = portfolio.purchases(SAMSUNG);

    ASSERT_PURCHASE(purchases[0], 5, 100.00, ArbitraryDate);
}

TEST_F(AFinancialPortfolio, IncludesSalesInPurchaseRecords)
{
    purchase(SAMSUNG, 10);
    sell(SAMSUNG, 5, 100.00, ArbitraryDate);
    
    auto sales = portfolio.purchases(SAMSUNG);

    ASSERT_PURCHASE(sales[1], -5, 100.00, ArbitraryDate);
}

bool operator==(const PurchaseRecord& lhs, const PurchaseRecord& rhs)
{
    return lhs.shareCount == rhs.shareCount && lhs.date == rhs.date;
}

TEST_F(AFinancialPortfolio, SeparatesPurchaseRecordsByTicker)
{
    purchase(SAMSUNG, 5);
    purchase(IBM, 1);

    auto sales = portfolio.purchases(SAMSUNG);
    ASSERT_THAT(sales, ElementsAre(PurchaseRecord(5, 0.0, ArbitraryDate)));
}

TEST_F(AFinancialPortfolio, 
        AnswersEmptyPurchaseRecordVectorWhenTickerNotFound)
{
    ASSERT_THAT(portfolio.purchases(SAMSUNG), 
                        Eq(std::vector<PurchaseRecord>()));
}

bool isSameDay(const date& lhs, const date& rhs)
{
    return lhs.year() == rhs.year() && lhs.month() == rhs.month()
            && lhs.day() == rhs.day();
}

TEST_F(AFinancialPortfolio, HasTodayAsTransactionDateWhenNotSpecified)
{
    portfolio.purchase(SAMSUNG, {10, 100.0});
    auto purchases = portfolio.purchases(SAMSUNG);

    ASSERT_THAT(purchases[0].date, Eq(TODAY));
}

TEST_F(AFinancialPortfolio, AnswersPriceOfSharesOnTransaction)
{
    purchase(IBM, 5, 100.0, ArbitraryDate);
    auto purchases = portfolio.purchases(IBM);

    ASSERT_THAT(purchases[0].priceOnTransaction, DoubleEq(100.00));
}

TEST_F(AFinancialPortfolio, AnswersAveragePurchasePriceOfGivenTicker)
{
    purchase(IBM, 5, 100.0, ArbitraryDate);
    purchase(IBM, 10, 200.0, ArbitraryDate);

    ASSERT_THAT(portfolio.averagePurchasePrice(IBM), 
                DoubleEq((100.0*5 + 200.0*10)/15));
}
