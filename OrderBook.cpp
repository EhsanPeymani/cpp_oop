#include "OrderBook.h"
#include "CSVReader.h"

#include <map>
#include <algorithm>

OrderBook::OrderBook(std::string filename)
{
    orders = CsvReader::readCsv(filename);

    // sort orders based on timestamp
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

void OrderBook::insertNewEntry(OrderBookEntry const& obe)
{
    orders.push_back(obe);

    // sort orders based on timestamp
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}


std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> productMap;

    for (auto const& entry : orders)
        productMap[entry.product] = true;

    for (auto const& item : productMap)
        products.push_back(item.first);

    return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                std::string const& product, 
                                                std::string const& timestamp 
                                                )
{
    std::vector<OrderBookEntry> subOrders;

    for (auto const& order : orders)
    {
        if (order.type == type && order.timestamp == timestamp && order.product == product)
            subOrders.push_back(order);
    }


    return subOrders;
}

unsigned int OrderBook::getNumberOfEntries()
{
    return orders.size();
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> const& orders)
{
    if (orders.size() == 0)
        return -1;

    double max = orders[0].price;

    for (auto const& order : orders)
    {
        if (order.price > max)
            max = order.price;
    }

    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> const& orders)
{
    if (orders.size() == 0)
        return -1;
        
    double min = orders[0].price;

    for (auto const& order : orders)
    {
        if (order.price < min)
            min = order.price;
    }

    return min;
}

double OrderBook::getAveragePrice(std::vector<OrderBookEntry> const& orders)
{
    return (getLowPrice(orders)+getHighPrice(orders))/2.0;
}

double OrderBook::getMedianPrice(std::vector<OrderBookEntry> const& orders)
{
    auto s = orders.size();

    std::vector<double> prices {};
    for (auto const& order : orders)
        prices.push_back(order.price);

    if (s == 0)
        return 0.0;

    if (s == 1)
        return prices[0];

    std::sort(prices.begin(), prices.end());

    unsigned int index = int(s/2);
    if (s%2 != 0)
        return prices[index];

    return 0.5*(prices[index] + prices[index-1]);
}

std::string OrderBook::getEarliestTime()
{
    // we should check to make sure that class member prders is not empty
    if (orders.size() == 0)
        return "";

    std::string earliestTimestamp = orders[0].timestamp;

    for (auto const& order : orders)
    {
        if (order.timestamp < earliestTimestamp)
            earliestTimestamp = order.timestamp;
    }
        
    return earliestTimestamp;
}

std::string OrderBook::getNextTime(std::string const& currentTimestamp)
{
    std::string nextTimestamp{};

    for (auto const& order : orders)
    {
        if (order.timestamp > currentTimestamp)
            {
                nextTimestamp = order.timestamp;
                break;
            }
    }

    if (nextTimestamp == "")
        nextTimestamp = getEarliestTime();

    return nextTimestamp;
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string const& product,
                                            std::string const& timestamp)
{
    std::vector<OrderBookEntry> sales {};

    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp); // want to sell
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp); // want to buy

    std::sort(asks.begin(), asks.end(), [](const OrderBookEntry& a, const OrderBookEntry& b)
                                            {return a.price < b.price;});
    std::sort(bids.begin(), bids.end(), [](const OrderBookEntry& a, const OrderBookEntry& b)
                                            {return a.price > b.price;});

    for (auto& ask : asks)
        for (auto& bid : bids)
            {
                if (bid.price < ask.price)
                    continue;

                OrderBookEntry sale{timestamp, product, OrderBookType::asksale, ask.price, 0};

                if (bid.username == "simuser")
                    sale.type = OrderBookType::bidsale;

                if (bid.username == "simuser" || ask.username == "simuser")
                    sale.username = "simuser";

                if (bid.amount == ask.amount && bid.amount > 0)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }

                if (bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount -= ask.amount;
                    break;
                }

                if (bid.amount < ask.amount)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount -= bid.amount;
                    bid.amount = 0;
                    continue;
                }
                
            }

    return sales;
}
