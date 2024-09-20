#include <iostream>
#include <string>
#include <vector>

#include "OrderBookEntry.h"

int main()
{
    OrderBookEntry obj1 {"2020/03/17 17:01:24.884492", "ETH/BTC", OrderBookType::bid, 2000, 0.1};
    OrderBookEntry obj2 {"2020/03/17 17:01:24.884492", "ETH/BTC", OrderBookType::bid, 1000, 0.1};
    OrderBookEntry obj3 {"2020/03/17 17:01:24.884492", "ETH/BTC", OrderBookType::bid, 4000, 0.1};
    OrderBookEntry obj4 {"2020/03/17 17:01:24.884492", "ETH/BTC", OrderBookType::bid, 3000, 0.1};

    std::vector<OrderBookEntry> enteries {obj1, obj2, obj3};
    enteries.push_back(obj4);

    std::cout << "Max Price: " << computeHighPrice(enteries) << std::endl;
    std::cout << "Min Price: " << computeLowPrice(enteries) << std::endl;
    std::cout << "Average Price: " << computeAveragePrice(enteries) << std::endl;
    std::cout << "Price Spread: " << computePriceSpread(enteries) << std::endl;
}