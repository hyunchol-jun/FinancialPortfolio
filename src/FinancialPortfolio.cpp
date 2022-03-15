#include "FinancialPortfolio.h"

bool FinancialPortfolio::isEmpty() const
{
    return 0 == m_holdings.size();
}

void FinancialPortfolio::purchase(const std::string& ticker, 
                                  PurchaseRecord&& record)
{
    transact(ticker, record);
}

void FinancialPortfolio::sell(const std::string& ticker, 
                              PurchaseRecord&& record)
{
    throwIfNotEnoughSharesToSell(ticker, record);
    negateShareCountForSale(record);
    transact(ticker, record);
}

void FinancialPortfolio::throwIfNotEnoughSharesToSell(
                                                const std::string& ticker,
                                                const PurchaseRecord& record)
{
    if (record.shareCount > shareCount(ticker)) 
        throw InsufficientSharesException();
}

void FinancialPortfolio::negateShareCountForSale(PurchaseRecord& record)
{
    record.shareCount = -record.shareCount;
}

void FinancialPortfolio::transact(const std::string& ticker, 
                                  const PurchaseRecord& record)
{
    throwIfShareCountIsZero(record.shareCount);
    addPurchaseRecord(ticker, record);
}

void FinancialPortfolio::throwIfShareCountIsZero(int shareChange) const
{
    if (shareChange == 0) throw ShareCountCannotBeZeroException();
}

void FinancialPortfolio::addPurchaseRecord(const std::string& ticker,
                                           const PurchaseRecord& record)
{
    if (!containsTicker(ticker))
        initializePurchaseRecords(ticker);
    add(ticker, record);
}

bool FinancialPortfolio::containsTicker(const std::string& ticker) const
{
    return m_holdings.find(ticker) != m_holdings.end();
}

void FinancialPortfolio::initializePurchaseRecords(const std::string& ticker)
{
    m_holdings[ticker] = Holding();
}

void FinancialPortfolio::add(const std::string& ticker, 
                             const PurchaseRecord& record)
{
    m_holdings[ticker].add(record);
}

std::vector<PurchaseRecord> 
        FinancialPortfolio::purchasesOfGivenTicker(const std::string& ticker) const 
{
    return find<Holding>(m_holdings, ticker).purchases();
}

int FinancialPortfolio::shareCount(const std::string& ticker) const
{
    return find<Holding>(m_holdings, ticker).shareCount();
}

double FinancialPortfolio::averagePurchasePrice(const std::string& ticker) const
{
    return find<Holding>(m_holdings, ticker).averagePurchasePrice();
}

double FinancialPortfolio::currentPriceOfShare(const std::string& ticker) const
{
    std::string response{yahooFinanceResponse(ticker)};
    return parsedCurrentPriceFromJson(response);
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
