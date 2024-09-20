#include "OrderBookEntry.h"

#include <string>
#include <map>

OrderBookEntry::OrderBookEntry(std::string _t, std::string _prod, OrderBookType _type, double _p, double _a, std::string const& _usern):
            price(_p), amount(_a), timestamp(_t), product(_prod), type(_type), username(_usern)
{};

OrderBookEntry::OrderBookEntry(std::string _t, std::string _prod, std::string _typeString, double _p, double _a, std::string const& _usern):
            price(_p), amount(_a), timestamp(_t), product(_prod), username(_usern)
{
    type = stringToOrderBookType(_typeString);
};


OrderBookType OrderBookEntry::stringToOrderBookType(std::string stringType)
{
    static const std::map<std::string, OrderBookType> typeMap =
    {
        {"bid", OrderBookType::bid},
        {"ask", OrderBookType::ask},
        {"bidsale", OrderBookType::bidsale},
        {"asksale", OrderBookType::asksale}
    };
    
    auto it = typeMap.find(stringType);
    if (it != typeMap.end())
    {
        return it->second;
    }

    return OrderBookType::unknown;
};

