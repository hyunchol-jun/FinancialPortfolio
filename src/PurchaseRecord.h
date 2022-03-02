#ifndef PurchaseRecord_h
#define PurchaseRecord_h
#include <boost/date_time/gregorian/gregorian_types.hpp>

struct PurchaseRecord {
    PurchaseRecord(int shareCount, 
            const boost::gregorian::date& date): 
        shareCount(shareCount), date(date) {}
    int shareCount;
    boost::gregorian::date date;
};
#endif
