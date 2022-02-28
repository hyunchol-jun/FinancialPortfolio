#include "FinancialPortfolio.h"

FinancialPortfolio::FinancialPortfolio() {}

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

void FinancialPortfolio::purchase(const std::string& ticker, int quantity)
{
    if (quantity <= 0) throw InvalidPurchaseException();
    m_holdings[ticker] += quantity;
}

int FinancialPortfolio::shareCount(const std::string& ticker)
{
    return m_holdings[ticker];
}

