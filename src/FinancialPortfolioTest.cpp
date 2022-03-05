#include "FinancialPortfolio.h"
#include "Http.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <iostream>
#include <memory>

using namespace ::testing;
using namespace boost::gregorian;


class AFinancialPortfolio: public Test
{
public:
    static const std::string IBM_JSON_RESPONSE;
    static const std::string ValidTicker;
    static const std::string ValidTimestamp;
    static const std::string ValidInterval;

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
const std::string AFinancialPortfolio::IBM_JSON_RESPONSE{
    R"delim({"chart":{"result":[{"meta":{"currency":"USD","symbol":"IBM","exchangeName":"NYQ","instrumentType":"EQUITY","firstTradeDate":-252322200,"regularMarketTime":1646427602,"gmtoffset":-18000,"timezone":"EST","exchangeTimezoneName":"America/New_York","regularMarketPrice":126.62,"chartPreviousClose":125.93,"priceHint":2,"currentTradingPeriod":{"pre":{"timezone":"EST","end":1646404200,"start":1646384400,"gmtoffset":-18000},"regular":{"timezone":"EST","end":1646427600,"start":1646404200,"gmtoffset":-18000},"post":{"timezone":"EST","end":1646442000,"start":1646427600,"gmtoffset":-18000}},"dataGranularity":"1d","range":"","validRanges":["1d","5d","1mo","3mo","6mo","1y","2y","5y","10y","ytd","max"]},"timestamp":[1646427602],"indicators":{"quote":[{"open":[124.4000015258789],"low":[124.21029663085938],"volume":[4301826],"high":[127.3499984741211],"close":[126.62000274658203]}],"adjclose":[{"adjclose":[126.62000274658203]}]}}],"error":null}})delim"
};
const std::string AFinancialPortfolio::ValidTicker{"IBM"};
const std::string AFinancialPortfolio::ValidTimestamp{"201646451256"};
const std::string AFinancialPortfolio::ValidInterval{"1d"};

class HttpStub: public Http
{
public:
    void initialize() override {}
    std::string get(const std::string& url) const override 
    {
        verify(url);
        return AFinancialPortfolio::IBM_JSON_RESPONSE;
    }
private: 
    void verify(const std::string& url) const 
    {
        std::string urlStart{
                "https://query1.finance.yahoo.com/v8/finance/chart/"};
        std::string expectedUrl{
            urlStart + AFinancialPortfolio::ValidTicker 
            + "?period1=%" + AFinancialPortfolio::ValidTimestamp 
            + "&period2=" + AFinancialPortfolio::ValidTimestamp 
            + "&interval=" + AFinancialPortfolio::ValidInterval
            + "&events=history"};
        ASSERT_THAT(url, Eq(expectedUrl));
    }
};

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
    return lhs.year() == rhs.year() && 
           lhs.month() == rhs.month() && 
           lhs.day() == rhs.day();
}

TEST_F(AFinancialPortfolio, AnswersTodayForTransactionDateWhenNotSpecified)
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
    sell(IBM, 3, 150.0, ArbitraryDate);

    ASSERT_THAT(portfolio.averagePurchasePrice(IBM), 
                DoubleEq((100.0*5 + 200.0*10 - 150.0*3)/(5 + 10 - 3)));
}

TEST_F(AFinancialPortfolio, AnswersCurrentPriceForTicker)
{
    HttpStub httpStub;
    portfolio.setHttp(std::make_shared<HttpStub>(httpStub));

    double price = portfolio.currentPriceOfShare(IBM);

    ASSERT_THAT(price, DoubleEq(126.62));
}

