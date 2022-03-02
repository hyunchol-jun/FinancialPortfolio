#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include "Holding.h"
#include "PurchaseRecord.h"
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
    int shareCount(const std::string& ticker) const;
    std::vector<PurchaseRecord> purchases(const std::string& ticker) const;

private:
    void transact(const std::string& ticker, int shareChange,
                  const boost::gregorian::date& transactionDate);
    void throwIfShareCountIsZero(int shareChange) const;
    void addPurchaseRecord(const std::string& ticker,
                           int shareChange, 
                           const boost::gregorian::date& date);
    bool containsTicker(const std::string& ticker) const;
    void initializePurchaseRecords(const std::string& ticker);
    void add(const std::string& ticker, PurchaseRecord&& record);
    template<typename T> 
    T find(std::unordered_map<std::string, T> map, const std::string& key) 
        const
    {
        auto it = map.find(key);
        return it == map.end() ?  T{} : it->second;
    }

    std::unordered_map<std::string, Holding> m_holdings;
};

#endif
