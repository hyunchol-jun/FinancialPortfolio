#include "FinancialPortfolio.h"

bool FinancialPortfolio::isEmpty() const
{
    for (const auto& account : m_accounts)
        if (!account.isEmpty()) return false;
    return true;
}

void FinancialPortfolio::addAccount(Account&& account)
{
    m_accounts.push_back(account);
}

void FinancialPortfolio::purchaseForAccount(const std::string& ticker, 
                                            PurchaseRecord&& record,
                                            Account& account)
{
    account.purchase(ticker, record);
}

void FinancialPortfolio::sellForAccount(const std::string& ticker, 
                                        PurchaseRecord&& record,
                                        Account& account)
{
    account.sell(ticker, record);
}

std::vector<PurchaseRecord> 
        FinancialPortfolio::purchasesOfGivenTicker(
                            const std::string& ticker) const 
{
    std::vector<PurchaseRecord> purchases;
    combineAllPurchaseRecordsOfTicker(purchases, ticker);
    return purchases; 
}

void FinancialPortfolio::combineAllPurchaseRecordsOfTicker(
                        std::vector<PurchaseRecord>& purchases, 
                        const std::string& ticker) const
{
    for (const auto& account: m_accounts)
        appendRecordsOf(account.purchasesOfGivenTicker(ticker), purchases);
}

void FinancialPortfolio::appendRecordsOf(
                const std::vector<PurchaseRecord>& records,
                std::vector<PurchaseRecord>& purchases) const
{
        purchases.insert(purchases.end(), records.begin(), records.end());
}


int FinancialPortfolio::shareCount(const std::string& ticker) const
{
    int totalShareCount{0};
    for (const auto& account: m_accounts)
        totalShareCount += account.shareCount(ticker);
    return totalShareCount;
}

double FinancialPortfolio::averagePurchasePrice(const std::string& ticker) const
{
    double sum{0.0};
    for (const auto& account: m_accounts)
        sum += account.averagePurchasePrice(ticker) 
               * account.shareCount(ticker);
    return sum / shareCount(ticker);
}

double FinancialPortfolio::currentPriceOfShare(const std::string& ticker) const
{
    std::string responseFromYahoo{yahooFinanceResponse(ticker)};
    return parsedCurrentPriceFromJson(responseFromYahoo);
}

double FinancialPortfolio::parsedCurrentPriceFromJson(
                                        const std::string& response) const
{
    try
    {
        return JsonParser{response}.currentPrice();
    } catch(std::exception exception){
        return 0.0;
    };
}

std::string FinancialPortfolio::yahooFinanceResponse(const std::string& ticker)
    const
{
    m_http->initialize();
    return m_http->get(yahooFinanceRequestUrl(ticker, "201646451256"));
}

std::string FinancialPortfolio::yahooFinanceRequestUrl(
                                    const std::string& ticker,
                                    const std::string& Timestamp) const
{
    std::string server{"https://query1.finance.yahoo.com/"};
    std::string document{"v8/finance/chart/"};
    return server + document + ticker + "?period1=%" + Timestamp
           + "&period2=" + Timestamp + "&interval=1d&events=history";
}

void FinancialPortfolio::setHttp(std::unique_ptr<Http> http)
{
    m_http = std::move(http);
}

std::vector<std::vector<std::string>> 
                FinancialPortfolio::holdingsConvertedToStringVector() const
{
    std::vector<std::vector<std::string>> resultVector;
    for (const auto& element: m_holdings)
    {
        resultVector.push_back(
            singleHoldingInStringVector(element.first, element.second));
    }
    return resultVector;
}

std::vector<std::string> FinancialPortfolio::singleHoldingInStringVector(
        const std::string& ticker, const Holding& holding) const
{
    std::vector<std::string> resultVector{ticker};
    int shareCount{holding.shareCount()};
    double avgPrice{holding.averagePurchasePrice()}, currPrice{125.0};
    resultVector.push_back(std::to_string(shareCount));
    resultVector.push_back(std::to_string(avgPrice));
    resultVector.push_back(std::to_string(currPrice));
    resultVector.push_back(
            std::to_string((currPrice-avgPrice) * shareCount));
    return resultVector;
}

std::vector<Account> FinancialPortfolio::accountsOfHolder(
                                        const std::string& holder) const
{
    Account temporaryAccount{};
    return std::vector<Account>{temporaryAccount};
}
