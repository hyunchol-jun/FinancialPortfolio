#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class InvalidPurchaseException: public std::exception
{
};

class InvalidSellException: public std::exception
{
};

struct PurchaseRecord {
    PurchaseRecord(unsigned int shareCount, 
            const boost::gregorian::date& date): 
        shareCount(shareCount), date(date) {}
    unsigned int shareCount;
    boost::gregorian::date date;
};

class FinancialPortfolio
{
public:
    static const boost::gregorian::date FIXED_PURCHASE_DATE;

    bool isEmpty() const;
    void purchase(const std::string& ticker, int quantity);
    void sell(const std::string& ticker, int quantity);
    int shareCount(const std::string& ticker);
    std::vector<PurchaseRecord> purchases(const std::string& ticker) const;

private:
    std::unordered_map<std::string, int> m_holdings{};
    std::vector<PurchaseRecord> m_purchases;
};

#endif
