#ifndef FinancialPortfolio_h
#define FinancialPortfolio_h
#include "Account.h"
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

class FinancialPortfolio
{
public:
    friend class WtPortfolioDisplayer;
    bool isEmpty() const;
    void purchaseForAccount(const std::string& ticker, 
                  PurchaseRecord&& record,
                  Account& account);
    void sellForAccount(const std::string& ticker, 
              PurchaseRecord&& record,
              Account& account);
    int shareCount(const std::string& ticker) const;
    std::vector<PurchaseRecord> purchasesOfGivenTicker(
                            const std::string& ticker) const;
    void combineAllPurchaseRecordsOfTicker(
                            std::vector<PurchaseRecord>& purchases,  
                            const std::string& ticker) const;
    double averagePurchasePrice(const std::string& ticker) const;
    void appendRecordsOf(const std::vector<PurchaseRecord>& records,
                            std::vector<PurchaseRecord>& purchases) const;
    double currentPriceOfShare(const std::string& ticker) const;
    void setHttp(std::unique_ptr<Http> http);
    std::vector<Account> accountsOfHolder(const std::string& holder) const;
    void addAccount(Account&& account);
    std::vector<Account> m_accounts;

private:
    std::string yahooFinanceResponse(const std::string& ticker) const;
    std::string yahooFinanceRequestUrl(const std::string& ticker,
                                 const std::string& Timestamp) const;
    double parsedCurrentPriceFromJson(const std::string& response) const;
    std::vector<std::vector<std::string>> 
                                holdingsInAllAccounts() const;
    void appendHoldingsVectorTo(
            std::vector<std::vector<std::string>>& totalHoldings,
            const std::vector<std::vector<std::string>>& holdingsFromAccount) 
            const;
    template<typename T> 
    T find(std::unordered_map<std::string, T> map, const std::string& key) 
        const
    {
        auto it = map.find(key);
        return it == map.end() ?  T{} : it->second;
    }

    std::unique_ptr<Http> m_http;
};

#endif
