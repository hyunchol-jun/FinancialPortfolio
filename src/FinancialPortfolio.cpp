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
        FinancialPortfolio::purchases(const std::string& ticker) const 
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
    std::string response{m_http->get(ticker)};
    nlohmann::json json = nlohmann::json::parse(response);
    return (double)json["chart"]["result"][0]["meta"]["regularMarketPrice"];
}

double FinancialPortfolio::convertedDoubleFromString(const std::string& string)
    const
{
    double result{};
    return successfullyConvertedFromString(string, result) ? result : 0.0;
}

void FinancialPortfolio::setHttp(const std::shared_ptr<Http>& http)
{
    m_http = http;
}
