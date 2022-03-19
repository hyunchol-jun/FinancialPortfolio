#include "Holding.h"

void Holding::add(const PurchaseRecord& record)
{
    m_purchaseRecords.push_back(record);
}

std::vector<PurchaseRecord> Holding::purchases() const
{
    return m_purchaseRecords;
}

int Holding::shareCount() const
{
    return std::accumulate(m_purchaseRecords.begin(), 
                           m_purchaseRecords.end(), 
                           0,
                [] (int total, PurchaseRecord record)
                {
                    return total + record.shareCount;
                });
}

double Holding::averagePurchasePrice() const
{
    if (shareCount() == 0) return 0.0;
    return totalPurchaseCost() / purchaseShareCount();
}

int Holding::purchaseShareCount() const
{
    return std::accumulate(m_purchaseRecords.begin(),
                           m_purchaseRecords.end(),
                           0,
               [] (int total, PurchaseRecord record)
               {
                    return (record.shareCount > 0) ?
                           (total + record.shareCount) : total;
               });
}

double Holding::totalPurchaseCost() const
{
    return std::accumulate(m_purchaseRecords.begin(),
                           m_purchaseRecords.end(),
                           0,
                   [](double total, const PurchaseRecord& record)
                   {
                        return (record.shareCount > 0) ? 
                               (total + (record.priceOnTransaction
                                         * record.shareCount)) : total;
                   });
}
