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
    static boost::gregorian::date TODAY;

    bool isEmpty() const;
    void purchase(const std::string& ticker, 
                  PurchaseRecord&& record);
    void sell(const std::string& ticker, 
              PurchaseRecord&& record);
    int shareCount(const std::string& ticker) const;
    std::vector<PurchaseRecord> purchases(const std::string& ticker) const;

private:
    void throwIfNotEnoughSharesToSell(const std::string& ticker,
                                      const PurchaseRecord& record);
    void negateShareCountForSale(PurchaseRecord& record);
    void transact(const std::string& ticker, const PurchaseRecord& record);
    void throwIfShareCountIsZero(int shareChange) const;
    void addPurchaseRecord(const std::string& ticker,
                           const PurchaseRecord& record);
    bool containsTicker(const std::string& ticker) const;
    void initializePurchaseRecords(const std::string& ticker);
    void add(const std::string& ticker, const PurchaseRecord& record);
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
