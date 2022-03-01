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
    if (quantity <= 0) throw InvalidPurchaseException();
    m_holdings[ticker] += quantity;
    m_purchases.push_back(PurchaseRecord(quantity, transactionDate));
}

std::vector<PurchaseRecord> 
        FinancialPortfolio::purchases(const std::string& ticker) const 
    {
        return m_purchases;
    }

void FinancialPortfolio::sell(const std::string& ticker, 
                              int quantity,
                              const boost::gregorian::date& transactionDate)
{
    if (quantity > shareCount(ticker)) throw InvalidSellException();
    m_holdings[ticker] -= quantity;
    m_purchases.push_back(PurchaseRecord(-quantity, transactionDate));
}

int FinancialPortfolio::shareCount(const std::string& ticker)
{
    auto iter = m_holdings.find(ticker);
    if (iter == m_holdings.end()) return 0;
    return iter->second;
}

