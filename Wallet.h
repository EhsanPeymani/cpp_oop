#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet
{
    public:
        Wallet() = default;

        void insertCurrency(std::string const& type, double amount);
        bool removeCurrency(std::string const& type, double amount);
        bool containsCurrency(std::string const& type, double amount) const;
        bool canFulfillOrder(OrderBookEntry const& obe) const;
        void processSale(OrderBookEntry const& sale);
        std::string toString() const;
        double getBalance(std::string const& type) const;

    private:
        std::map<std::string, double> currencies;
};