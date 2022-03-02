#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class ShareCountCannotBeZeroException: public std::exception
{
};

class InsufficientSharesException: public std::exception
{
};

struct PurchaseRecord {
    PurchaseRecord(int shareCount, 
            const boost::gregorian::date& date): 
        shareCount(shareCount), date(date) {}
    int shareCount;
    boost::gregorian::date date;
};

class FinancialPortfolio
{
public:
    static const boost::gregorian::date FIXED_PURCHASE_DATE;

    bool isEmpty() const;
    void purchase(const std::string& ticker, 
                  int quantity,
                  const boost::gregorian::date& transactionDate=
                  FIXED_PURCHASE_DATE);
    void sell(const std::string& ticker, int quantity,
                  const boost::gregorian::date& transactionDate=
                  FIXED_PURCHASE_DATE);
    void transact(const std::string& ticker, int shareChange,
                  const boost::gregorian::date& transactionDate);
    void throwIfShareCountIsZero(int shareChange) const;
    void updateShareChange(const std::string& ticker, int shareChange);
    void addPurchaseRecord(int shareChange, 
                           const boost::gregorian::date& date);
    int shareCount(const std::string& ticker);
    std::vector<PurchaseRecord> purchases(const std::string& ticker) const;

private:
    std::unordered_map<std::string, int> m_holdings{};
    std::vector<PurchaseRecord> m_purchases;
};

#endif
