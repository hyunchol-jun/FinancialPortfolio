#include "FinancialPortfolio.h"
using namespace boost::gregorian;

const date FinancialPortfolio::FIXED_PURCHASE_DATE(date(2014, Jan, 1));

bool FinancialPortfolio::isEmpty() const
{
    auto iter{m_holdings.begin()};
    while (iter != m_holdings.end())
    {
        if (iter->second != 0) return false;
        ++iter;
    }
    return true;
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
    updateShareChange(ticker, shareChange);
    addPurchaseRecord(shareChange, transactionDate);
}

void FinancialPortfolio::throwIfShareCountIsZero(int shareChange) const
{
    if (shareChange == 0) throw ShareCountCannotBeZeroException();
}

void FinancialPortfolio::updateShareChange(const std::string& ticker, 
                                           int shareChange)
{
    m_holdings[ticker] += shareChange;
}

void FinancialPortfolio::addPurchaseRecord(int shareChange, 
                                           const date& date)
{
    m_purchases.push_back(PurchaseRecord(shareChange, date));
}

std::vector<PurchaseRecord> 
        FinancialPortfolio::purchases(const std::string& ticker) const 
    {
        return m_purchases;
    }

int FinancialPortfolio::shareCount(const std::string& ticker)
{
    auto iter = m_holdings.find(ticker);
    if (iter == m_holdings.end()) return 0;
    return iter->second;
}

