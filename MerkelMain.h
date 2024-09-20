#pragma once

#include <vector>
#include <tuple>

#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
    public:
        MerkelMain() = default;

        /** Call this to start */
        void init();

    private:
        void enterAsk();
        void enterBid();
        void printMarketStats();
        void printWallet();
        void gotoNextTimeFrame();
        void invalidInput();
        void printMenu();
        void printHelp();
        void processUserOption(int userOption);
        int getUserOption();

        void userInputToOrderBookEntry(std::string const& input, 
                                       std::string const& type,
                                       OrderBookEntry& obe);
        void enterUserOffer(std::string const& type);

        OrderBook orderBook{};
        std::string currentTime{};
        Wallet wallet{};
};