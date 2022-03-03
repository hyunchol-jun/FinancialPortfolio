#include "FinancialPortfolio.h"
using namespace boost::gregorian;

date FinancialPortfolio::TODAY(boost::gregorian::day_clock::local_day());

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
    if (record.shareCount > shareCount(ticker)) 
        throw InsufficientSharesException();
    record.shareCount = -record.shareCount;
    transact(ticker, record);
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

