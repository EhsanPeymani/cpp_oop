#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"

class CsvReader
{
    public:
        CsvReader();

        /** a stateless function - so we do not need to instantiate the class */
        static std::vector<OrderBookEntry> readCsv(std::string filename);
        static unsigned int stringToUnsignedInt(std::string const& val);
        static std::vector<std::string> tokenize(std::string csvLine, char separator);
        static std::tuple<double, double> getPriceAndAmount(std::string const& priceString, std::string const& amountString);

    private:
        static double stringToDouble(std::string const& val);
        static OrderBookEntry stringsToObe(std::vector<std::string> tokens);
};