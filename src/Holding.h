#ifndef Holding_h
#define Holding_h
#include "PurchaseRecord.h"
#include <numeric>
#include <vector>

class Holding
{
public:
    void add(PurchaseRecord& record)
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
                               m_purchaseRecords.end(), 0,
            [] (int total, PurchaseRecord record)
            {
                return total + record.shareCount;
            });
    }
private:
    std::vector<PurchaseRecord> m_purchaseRecords;
};
#endif
