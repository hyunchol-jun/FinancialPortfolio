#ifndef PurchaseRecord_h
#define PurchaseRecord_h
#include "Date.h"

struct PurchaseRecord {
    PurchaseRecord(int shareCount, 
                   double priceOnTransaction,
                   const Date& date=Date{}) :
                   shareCount(shareCount), 
                   priceOnTransaction(priceOnTransaction),
                   date(date) {}
    int shareCount;
    double priceOnTransaction;
    Date date;
};
#endif
