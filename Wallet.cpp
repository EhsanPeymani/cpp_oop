#include "Wallet.h"
#include "CSVReader.h"

#include <sstream>
#include <iomanip>
#include <iostream>


void Wallet::insertCurrency(std::string const& type, double amount)
{
    std::cout << "Inserting: " << amount << " " << type << std::endl;
    
    if (amount < 0)
        throw std::invalid_argument("amount shall be non-negative value");

    currencies[type] += amount;
}

bool Wallet::removeCurrency(std::string const& type, double amount)
{
    std::cout << "Request to remove " << amount << " " << type << std::endl;

    if (amount < 0)
        throw std::invalid_argument("amount shall be non-negative value");

    if (containsCurrency(type, amount))
        {
            std::cout << "Removing " << type << ": " << amount << std::endl;
            currencies[type] -= amount;
            return true;
        }
    return false;
}

bool Wallet::containsCurrency(std::string const& type, double amount) const
{
    if (currencies.count(type) == 0)
        return false;
    return currencies.at(type) >= amount;
}

bool Wallet::canFulfillOrder(OrderBookEntry const& obe) const
{
    std::vector<std::string> currs = CsvReader::tokenize(obe.product, '/');

    std::cout << "Wallet: " << std::endl;
    std::cout << toString() << std::endl;
    
    if (obe.type == OrderBookType::ask)
    {
        double requiredAmount = obe.amount;
        return containsCurrency(currs[0], requiredAmount);
    }

    if (obe.type == OrderBookType::bid)
    {
        double requiredAmount = obe.amount * obe.price;
        return containsCurrency(currs[1], requiredAmount);
    }

    return false;
}

std::string Wallet::toString() const
{
    std::stringstream ss;
    for (const auto& c : currencies)
    {
        ss << "** " << std::quoted(c.first) << ": " << c.second << std::endl;
    }
    return ss.str();
}

double Wallet::getBalance(std::string const& type) const
{
    if (currencies.count(type) == 0)
        return 0.0;
    
    return currencies.at(type);
}

void Wallet::processSale(OrderBookEntry const& sale)
{
    std::vector<std::string> currs = CsvReader::tokenize(sale.product, '/');

    double outgoingAmount{}, incomingAmount{};
    std::string outgoingCurrency{}, incomingCurrency{};

    if (sale.type == OrderBookType::asksale)
    {
        outgoingAmount = sale.amount;
        outgoingCurrency = currs[0];
        incomingAmount = sale.amount * sale.price;
        incomingCurrency = currs[1];
    }

    if (sale.type == OrderBookType::bidsale)
    {
        incomingAmount = sale.amount;
        incomingCurrency = currs[0];
        outgoingAmount = sale.amount * sale.price;
        outgoingCurrency = currs[1];
    }

    currencies[incomingCurrency] += incomingAmount;
    currencies[outgoingCurrency] -= outgoingAmount;

}
