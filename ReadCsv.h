#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"

class ReadCsv
{
    public:
        ReadCsv(std::string _fileName);

        /** Read the csv file and populate the data in a vector of related object */
        bool readFile();

        /** Returns read objects from the csv file */
        std::vector<OrderBookEntry> const& getObjects();
        
    private:
        std::vector<std::string> tokenize(std::string csvLine, char separator);
        double stringToDouble(std::string const& val);

        std::string fileName;
        std::vector<OrderBookEntry> objects;
};