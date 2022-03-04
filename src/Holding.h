#ifndef Holding_h
#define Holding_h
#include "PurchaseRecord.h"
#include <numeric>
#include <vector>

class Holding
{
public:
    void add(const PurchaseRecord& record)
    {
        m_purchaseRecords.push_back(record);
    }

    std::vector<PurchaseRecord> purchases() const
    {
        return m_purchaseRecords;
    }

    int shareCount() const
    {
        return std::accumulate(m_purchaseRecords.begin(), 
                               m_purchaseRecords.end(), 
                               0,
            [] (int total, PurchaseRecord record)
            {
                return total + record.shareCount;
            });
    }

    double averagePurchasePrice() const
    {
        double total = std::accumulate(m_purchaseRecords.begin(),
                                       m_purchaseRecords.end(),
                                       0,
                               [](double total, PurchaseRecord record)
                               {
                                    return total 
                                            + (record.priceOnTransaction
                                            * record.shareCount);
                               });
        return total / shareCount();
    }

private:
    std::vector<PurchaseRecord> m_purchaseRecords;
};
#endif
