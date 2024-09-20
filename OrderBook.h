#pragma once

#include <string>
#include <vector>

#include "OrderBookEntry.h"

class OrderBook
{
    public:
        OrderBook() = default;
        /** construct, reading a csv data file */
        OrderBook(std::string filename);

        /** return vector of all unique products in the data set*/
        std::vector<std::string> getKnownProducts();

        /** return vector of Orders which matches the filters */
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                                std::string const& product, 
                                                std::string const& timestamp 
                                                );
        /** returns the earliest tme in the order book */
        std::string getEarliestTime();

        /** returns the next time after the sent time in the order book */
        std::string getNextTime(std::string const& currentTimestamp);

        std::vector<OrderBookEntry> matchAsksToBids(std::string const& product,
                                                    std::string const& timestamp);
        
        unsigned int getNumberOfEntries();

        void insertNewEntry(OrderBookEntry const& obe);
        
        static double getHighPrice(std::vector<OrderBookEntry> const&);
        static double getLowPrice(std::vector<OrderBookEntry> const&);
        /** Calculate the average of the prices for the given set */
        static double getAveragePrice(std::vector<OrderBookEntry> const&);
        /** Calculate the mean of the prices for the given set */
        static double getMedianPrice(std::vector<OrderBookEntry> const&);

    private:
        std::vector<OrderBookEntry> orders;
};