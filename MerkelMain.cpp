#include <iostream>
#include <map>
#include <functional>
#include <limits>
#include <iomanip>

#include "MerkelMain.h"
#include "CSVReader.h"

void MerkelMain::init()
{

    std::string filename = "data.csv";
    orderBook = OrderBook{filename};
    std::cout << "Total Number of Entries: " << orderBook.getNumberOfEntries() << std::endl;

    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 100.0);
    wallet.insertCurrency("NOK", 20.0);
    
    std::cout << "Wallet At this time: " << std::endl;
    std::cout << wallet.toString() << std::endl;

    int userOption {};
    while(true)
    {
        printMenu();
        userOption = getUserOption();
        processUserOption(userOption);
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask: product,price,amount, eg.. ETH/BTC,0.1,100" << std::endl;
    enterUserOffer("ask");
}

void MerkelMain::enterBid()
{
    std::cout << "Make a bid: product,price,amount, eg.. ETH/BTC,0.1,100" << std::endl;
    enterUserOffer("bid");
}

void MerkelMain::enterUserOffer(std::string const& type)
{
    std::string input;
    std::getline(std::cin, input);

    try
    {
        OrderBookEntry obe {};
        userInputToOrderBookEntry(input, type, obe);
        if (wallet.canFulfillOrder(obe))
        {
            orderBook.insertNewEntry(obe);
            std::cout << "Order successfully added" << std::endl;
        }
        else
            std::cout << "Order cannot be fulfilled." << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cout << "MerkelMain::enterUserOffer: Error in conversion of user input: " << 
                    "type: " << std::quoted(type) << " " <<
                    e.what() << std::endl;
    }
}

void MerkelMain::userInputToOrderBookEntry(std::string const& input, 
                                           std::string const& type,
                                           OrderBookEntry& obe)
{
    std::vector<std::string> tokens = CsvReader::tokenize(input, ',');

    if (tokens.size() != 3)
        throw std::invalid_argument("Cannot find 3 tokens in input");

    auto [price, amount] = CsvReader::getPriceAndAmount(tokens[1], tokens[2]);

    obe = OrderBookEntry {currentTime, tokens[0], type, price, amount, "simuser"};
}

void MerkelMain::printMarketStats()
{
    for (auto const& product : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << product << std::endl;
        auto entries = orderBook.getOrders(OrderBookType::ask, product, currentTime);
        std::cout << "Total Number of Asks: " << entries.size() << std::endl;
        std::cout << "Max Price: " << OrderBook::getHighPrice(entries) 
                    << ", Min Price: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Average Price: " << OrderBook::getAveragePrice(entries) 
                    << ", Median Price: " << OrderBook::getMedianPrice(entries) << std::endl;
    }
}

void MerkelMain::printWallet()
{
    std::cout << "Wallet At this time: " << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeFrame()
{
    std::cout << "Going to next time frame, Nr orders: " << orderBook.getNumberOfEntries() << std::endl;

    for (auto const& product : orderBook.getKnownProducts())
    {
        std::cout << "Matching " << std::quoted(product) << std::endl;
        auto sales = orderBook.matchAsksToBids(product, currentTime);

        std::cout << "Sales: " << sales.size() << std::endl;
        for (auto const& sale : sales)
        {
            std::cout << "Sale Price: " << sale.price 
                      << ", amount: " << sale.amount 
                      << ", user: " << sale.username 
                      << std::endl;
            if (sale.username == "simuser")
                wallet.processSale(sale);
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::invalidInput()
{
    std::cout << "Invalid choice. Choose 1-6" << std::endl;
}

void MerkelMain::printMenu()
{
    std::cout << "1: Print help " << std::endl;
    std::cout << "2: Print exchange stats " << std::endl;
    std::cout << "3: Make an ask " << std::endl;
    std::cout << "4: Make a bid " << std::endl;
    std::cout << "5: Print wallet " << std::endl;
    std::cout << "6: Continue " << std::endl;

    std::cout << "=============================" << std::endl;
    std::cout << "Current time: " << currentTime << std::endl;
    std::cout << "Type in 1-6:" << std::endl;

}

int MerkelMain::getUserOption()
{
    int userOption {};

    try
    {
        std::string userInput {};
        std::getline(std::cin, userInput);
        userOption = CsvReader::stringToUnsignedInt(userInput);
        std::cout << "You chose: " << userOption << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "**** Not a valid input **** " << userOption << std::endl;
    }
    
    return userOption;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - choose options from the menu" << std::endl;
    std::cout << "and follow the on screen instructions." << std::endl;
}

void MerkelMain::processUserOption(int userOption)
{
    std::map<int, std::function<void()>> menu;

    menu[1] = std::bind(&MerkelMain::printHelp, this);
    menu[2] = std::bind(&MerkelMain::printMarketStats, this);
    menu[3] = std::bind(&MerkelMain::enterAsk, this);
    menu[4] = std::bind(&MerkelMain::enterBid, this);
    menu[5] = std::bind(&MerkelMain::printWallet, this);
    menu[6] = std::bind(&MerkelMain::gotoNextTimeFrame, this);

    if (userOption > 6 || userOption <1)
        {
            invalidInput();
            return;
        }

    menu[userOption]();
}