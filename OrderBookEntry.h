#pragma once

#include <string>

enum class OrderBookType
{
    bid,
    ask,
    bidsale,
    asksale,
    unknown
};


class OrderBookEntry
{
    public:
        OrderBookEntry() = default;
        OrderBookEntry(std::string _t, std::string _prod, OrderBookType _type, double _p, double _a, std::string const& _username = "dataset");
        OrderBookEntry(std::string _t, std::string _prod, std::string _type, double _p, double _a, std::string const& _username = "dataset");

        static OrderBookType stringToOrderBookType(std::string stringType);
        static bool compareByTimestamp(const OrderBookEntry& a, const OrderBookEntry& b)
        {
            return a.timestamp < b.timestamp;
        }

        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType type;
        std::string username;
};