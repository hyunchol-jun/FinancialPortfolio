#include "FinancialPortfolio.h"

FinancialPortfolio::FinancialPortfolio() : m_shareCount{0} {}

bool FinancialPortfolio::isEmpty() const
{
    return 0 == m_shareCount;
}

void FinancialPortfolio::purchase(const std::string& ticker, int quantity)
{
    if (quantity <= 0) throw InvalidPurchaseException();
    m_shareCount += quantity;
}

int FinancialPortfolio::shareCount(const std::string& ticker)
{
    return m_shareCount;
}

