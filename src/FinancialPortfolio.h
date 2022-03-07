#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include "Http.h"
#include "Holding.h"
#include "JsonParser.h"
#include "PurchaseRecord.h"
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
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
    bool isEmpty() const;
    void purchase(const std::string& ticker, 
                  PurchaseRecord&& record);
    void sell(const std::string& ticker, 
              PurchaseRecord&& record);
    int shareCount(const std::string& ticker) const;
    std::vector<PurchaseRecord> purchasesOfGivenTicker(const std::string& ticker) const;
    double averagePurchasePrice(const std::string& ticker) const;
    double currentPriceOfShare(const std::string& ticker) const;
    void setHttp(std::unique_ptr<Http> http);

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
    std::string yahooFinanceResponse(const std::string& ticker) const;
    std::string yahooFinanceRequestUrl(const std::string& ticker,
                                 const std::string& Timestamp) const;
    double parsedCurrentPriceFromJson(const std::string& response) const;
    template<typename T> 
    T find(std::unordered_map<std::string, T> map, const std::string& key) 
        const
    {
        auto it = map.find(key);
        return it == map.end() ?  T{} : it->second;
    }

    std::unordered_map<std::string, Holding> m_holdings;
    std::unique_ptr<Http> m_http;
};

#endif
