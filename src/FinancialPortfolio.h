#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include <string>

class FinancialPortfolio
{
public:
    FinancialPortfolio()
    {
        m_empty = true;
    }

    bool isEmpty() const
    {
        return m_empty;
    }

    void purchase(const std::string& ticker, int quantity)
    {
        m_empty = false;
    }

    int shareCount(const std::string& ticker)
    {
        return 0;
    }

private:
    bool m_empty{};
};

#endif
