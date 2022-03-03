#ifndef PurchaseRecord_h
#define PurchaseRecord_h
#include <boost/date_time/gregorian/gregorian_types.hpp>
using namespace boost::gregorian;

struct PurchaseRecord {
    PurchaseRecord(int shareCount, 
                   double priceOnTransaction,
                   const boost::gregorian::date& date=day_clock::local_day()) :
                   shareCount(shareCount), 
                   priceOnTransaction(priceOnTransaction),
                   date(date) {}
    int shareCount;
    double priceOnTransaction;
    boost::gregorian::date date{};
};
#endif
