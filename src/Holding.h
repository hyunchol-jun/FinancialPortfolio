#ifndef Holding_h
#define Holding_h
#include "PurchaseRecord.h"
#include <numeric>
#include <vector>

class Holding
{
public:
    void add(const PurchaseRecord& record);
    std::vector<PurchaseRecord> purchases() const;
    int shareCount() const;
    double averagePurchasePrice() const;

private:
    std::vector<PurchaseRecord> m_purchaseRecords;
    double totalValue() const;
};
#endif
