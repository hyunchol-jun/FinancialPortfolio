#include "Date.h"
#include "FinancialPortfolio.h"
#include "Http.h"
#include "HttpStub.h"
#include "PurchaseRecord.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <memory>

using namespace ::testing;


class AFinancialPortfolio: public Test
{
public:
    static const std::string ValidTicker;
    static const std::string ValidTimestamp;
    static const std::string ValidInterval;

    AFinancialPortfolio()
    {
        portfolio.addAccount(Account{});
    }

protected:
    static const std::string IBM;
    static const std::string SAMSUNG;
    FinancialPortfolio portfolio;
    static const Date ArbitraryDate;
    static const Date TODAY;

    void purchase(const std::string& ticker,
                  int shareCount,
                  double purchasePrice=0.0,
                  const Date& 
                        transactionDate=AFinancialPortfolio::ArbitraryDate)
    {
        portfolio.purchaseForAccount(ticker, 
                           {shareCount, purchasePrice, transactionDate},
                           portfolio.m_accounts[0]);
    }

    void sell(const std::string& ticker,
              int shareCount,
              double salePrice=0.0,
              const Date&
                        transactionDate=AFinancialPortfolio::ArbitraryDate)

    {
        portfolio.sellForAccount(ticker,
                            {shareCount, salePrice, transactionDate},
                            portfolio.m_accounts[0]);
    }

    void ASSERT_PURCHASE(PurchaseRecord& purchase, 
                         int shareCount, 
                         double purchasePrice,
                         const Date& date)
    {
        ASSERT_THAT(purchase.shareCount, Eq(shareCount));
        ASSERT_THAT(purchase.priceOnTransaction, DoubleEq(purchasePrice));
        ASSERT_THAT(purchase.date, Eq(date));
    }
};
const Date AFinancialPortfolio::TODAY{};
const Date AFinancialPortfolio::ArbitraryDate(2014, Date::Sep, 5);
const std::string AFinancialPortfolio::IBM{"IBM"};
const std::string AFinancialPortfolio::SAMSUNG{"SSNLF"};
const std::string AFinancialPortfolio::ValidTicker{"IBM"};
const std::string AFinancialPortfolio::ValidTimestamp{"201646451256"};
const std::string AFinancialPortfolio::ValidInterval{"1d"};

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

    auto purchases = portfolio.purchasesOfGivenTicker(SAMSUNG);

    ASSERT_PURCHASE(purchases[0], 5, 100.00, ArbitraryDate);
}

TEST_F(AFinancialPortfolio, IncludesSalesInPurchaseRecords)
{
    purchase(SAMSUNG, 10);
    sell(SAMSUNG, 5, 100.00, ArbitraryDate);
    
    auto sales = portfolio.purchasesOfGivenTicker(SAMSUNG);

    ASSERT_PURCHASE(sales[1], -5, 100.00, ArbitraryDate);
}

bool operator==(const Date& lhs, const Date& rhs)
{
    return lhs.m_date == rhs.m_date;
}

bool operator==(const PurchaseRecord& lhs, const PurchaseRecord& rhs)
{
    return lhs.shareCount == rhs.shareCount && lhs.date == rhs.date;
}

TEST_F(AFinancialPortfolio, SeparatesPurchaseRecordsByTicker)
{
    purchase(SAMSUNG, 5);
    purchase(IBM, 1);

    auto sales = portfolio.purchasesOfGivenTicker(SAMSUNG);
    ASSERT_THAT(sales, ElementsAre(PurchaseRecord(5, 0.0, ArbitraryDate)));
}

TEST_F(AFinancialPortfolio, 
        AnswersEmptyPurchaseRecordVectorWhenTickerNotFound)
{
    ASSERT_THAT(portfolio.purchasesOfGivenTicker(SAMSUNG), 
                        Eq(std::vector<PurchaseRecord>()));
}

TEST_F(AFinancialPortfolio, AnswersTodayForTransactionDateWhenNotSpecified)
{
    portfolio.purchaseForAccount(SAMSUNG, {10, 100.0}, portfolio.m_accounts[0]);
    auto purchases = portfolio.purchasesOfGivenTicker(SAMSUNG);

    ASSERT_THAT(purchases[0].date, Eq(TODAY));
}

TEST_F(AFinancialPortfolio, AnswersPriceOfSharesOnTransaction)
{
    purchase(IBM, 5, 100.0, ArbitraryDate);
    auto purchases = portfolio.purchasesOfGivenTicker(IBM);

    ASSERT_THAT(purchases[0].priceOnTransaction, DoubleEq(100.00));
}

TEST_F(AFinancialPortfolio, AnswersAveragePurchasePriceOfGivenTicker)
{
    purchase(IBM, 5, 100.0, ArbitraryDate);
    purchase(IBM, 10, 200.0, ArbitraryDate);
    sell(IBM, 3, 150.0, ArbitraryDate);

    ASSERT_THAT(portfolio.averagePurchasePrice(IBM), 
                DoubleEq((100.0*5 + 200.0*10) / (5 + 10)));
}

TEST_F(AFinancialPortfolio, MakesHttpRequestToObtainCurrenPriceOfShare)
{
    std::unique_ptr<HttpStub> ptr{new HttpStub};
    InSequence forceExpectationOrder;
    std::string urlStart{
            "https://query1.finance.yahoo.com/v8/finance/chart/"};
    std::string expectedUrl = urlStart + AFinancialPortfolio::ValidTicker 
                            + "?period1=%" + AFinancialPortfolio::ValidTimestamp 
                            + "&period2=" + AFinancialPortfolio::ValidTimestamp 
                            + "&interval=" + AFinancialPortfolio::ValidInterval
                            + "&events=history";

    EXPECT_CALL(*ptr, initialize());
    EXPECT_CALL(*ptr, get(expectedUrl));
    
    portfolio.setHttp(std::move(ptr));
    portfolio.currentPriceOfShare(IBM);
}

TEST_F(AFinancialPortfolio, ExtractsCurrentPriceFromRetrievedJson)
{
    std::unique_ptr<NiceMock<HttpStub>> ptr{new NiceMock<HttpStub>};
    EXPECT_CALL(*ptr, get(_)).WillOnce(Return(
    R"delim({"chart":{"result":[{"meta":{"currency":"USD","symbol":"IBM","exchangeName":"NYQ","instrumentType":"EQUITY","firstTradeDate":-252322200,"regularMarketTime":1646427602,"gmtoffset":-18000,"timezone":"EST","exchangeTimezoneName":"America/New_York","regularMarketPrice":126.62,"chartPreviousClose":125.93,"priceHint":2,"currentTradingPeriod":{"pre":{"timezone":"EST","end":1646404200,"start":1646384400,"gmtoffset":-18000},"regular":{"timezone":"EST","end":1646427600,"start":1646404200,"gmtoffset":-18000},"post":{"timezone":"EST","end":1646442000,"start":1646427600,"gmtoffset":-18000}},"dataGranularity":"1d","range":"","validRanges":["1d","5d","1mo","3mo","6mo","1y","2y","5y","10y","ytd","max"]},"timestamp":[1646427602],"indicators":{"quote":[{"open":[124.4000015258789],"low":[124.21029663085938],"volume":[4301826],"high":[127.3499984741211],"close":[126.62000274658203]}],"adjclose":[{"adjclose":[126.62000274658203]}]}}],"error":null}})delim"));
    portfolio.setHttp(std::move(ptr));
    
    double price = portfolio.currentPriceOfShare(IBM);

    ASSERT_THAT(price, DoubleEq(126.62));
}

TEST_F(AFinancialPortfolio, ReturnsZeroWhenPriceRetrievingFailed)
{
    std::unique_ptr<NiceMock<HttpStub>> ptr{new NiceMock<HttpStub>};
    portfolio.setHttp(std::move(ptr));

    double price = portfolio.currentPriceOfShare("");

    ASSERT_THAT(price, DoubleEq(0.0)); 
}

TEST_F(AFinancialPortfolio, AnswersInfoForASingleAccount)
{
    auto accounts = portfolio.accountsOfHolder("Chol");
    auto account = accounts[0];

    ASSERT_THAT(account.holderName(), Eq("Chol"));
    ASSERT_THAT(account.brokerName(), Eq("IBKR"));
    ASSERT_THAT(account.accountType(), Eq("TFSA"));
}
