#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include <string>
#include <map>

class InvalidPurchaseException: public std::exception
{

};

class FinancialPortfolio
{
public:
    FinancialPortfolio();

    bool isEmpty() const;
    void purchase(const std::string& ticker, int quantity);
    int shareCount(const std::string& ticker);

private:
    std::map<std::string, int> m_holdings{};
};

#endif
