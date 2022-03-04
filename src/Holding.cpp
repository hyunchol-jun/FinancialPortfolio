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
    return totalValue() / shareCount();
}

double Holding::totalValue() const
{
    return std::accumulate(m_purchaseRecords.begin(),
                                   m_purchaseRecords.end(),
                                   0,
                           [](double total, PurchaseRecord record)
                           {
                                return total 
                                        + (record.priceOnTransaction
                                        * record.shareCount);
                           });
}
