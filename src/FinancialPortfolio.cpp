#include "FinancialPortfolio.h"
using namespace boost::gregorian;

const date FinancialPortfolio::FIXED_PURCHASE_DATE(date(2014, Jan, 1));

bool FinancialPortfolio::isEmpty() const
{
    return 0 == m_holdings.size();
}

void FinancialPortfolio::purchase(
    const std::string& ticker, int quantity, const date& transactionDate)
{
    transact(ticker, quantity, transactionDate);
}

void FinancialPortfolio::sell(
    const std::string& ticker, int quantity, const date& transactionDate)
{
    if (quantity > shareCount(ticker)) throw InsufficientSharesException();
    transact(ticker, -quantity, transactionDate);
}

void FinancialPortfolio::transact(const std::string& ticker, 
                                  int shareChange,
                                  const date& transactionDate)
{
    throwIfShareCountIsZero(shareChange);
    addPurchaseRecord(ticker, shareChange, transactionDate);
}

void FinancialPortfolio::throwIfShareCountIsZero(int shareChange) const
{
    if (shareChange == 0) throw ShareCountCannotBeZeroException();
}

void FinancialPortfolio::addPurchaseRecord(const std::string& ticker,
                                           int shareChange, 
                                           const date& date)
{
    if (!containsTicker(ticker))
        initializePurchaseRecords(ticker);
    add(ticker, {shareChange, date});
}

bool FinancialPortfolio::containsTicker(const std::string& ticker) const
{
    return m_holdings.find(ticker) != m_holdings.end();
}

void FinancialPortfolio::initializePurchaseRecords(const std::string& ticker)
{
    m_holdings[ticker] = Holding();
}

void FinancialPortfolio::add(const std::string& ticker, PurchaseRecord&& record)
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

