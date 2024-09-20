#include "CSVReader.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <iomanip>


std::vector<OrderBookEntry> CsvReader::readCsv(std::string filename)
{
    std::ifstream file {filename};
    std::string line {};
    std::vector<OrderBookEntry> entries{};
    signed int count{};

    if (file.is_open())
    {
        std::cout << "file " << filename << " opened successfully." << std::endl;
        unsigned int count {};

        while (std::getline(file, line))
        {
            ++count;
            try{
                std::vector<std::string> tokens = tokenize(line, ',');
                OrderBookEntry obe = stringsToObe(tokens);

                entries.push_back(obe);
            }
            catch(const std::exception& e)
            {
                std::cout << "CsvReader::readCsv skip LINE " << count << std::endl;
            }
        }

        std::cout << "File read completely. Number of entries: " << entries.size() << std::endl;

        file.close();
        std::cout << "File closed gracefully" << std::endl;

        return entries;
    }
    else
    {
        std::cout << "Problem opening the file " << filename << std::endl;
        return entries;
    }

}

std::vector<std::string> CsvReader::tokenize(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token {};

    start = csvLine.find_first_not_of(separator, 0);

    do
    {
        end = csvLine.find_first_of(separator, start);

        if (start == end || start == csvLine.length())
        {
            std::cout   << "Breaking tokenizing. start: " << start 
                        << ", end: " << end 
                        << ", Length: " << csvLine.length() << std::endl;
            continue;;
        }

        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);

        tokens.push_back(token);
        start = end + 1;

    } while (end != std::string::npos);
    
    return tokens;
}

OrderBookEntry CsvReader::stringsToObe(std::vector<std::string> tokens)
{
    if (tokens.size() != 5)
    {
        std::cout << "CsvReader::stringsToObe  Bad input" << std::endl;
        throw std::invalid_argument("CsvReader::stringsToObe: The string vector size is not 5");
    }

    auto [price, amount] = getPriceAndAmount(tokens[3], tokens[4]);

    auto obe = OrderBookEntry {tokens[0], tokens[1], tokens[2], price, amount};
    return obe;
}

double CsvReader::stringToDouble(std::string const& val)
{
    double result {};
    size_t idx {};

    try
    {
        result = std::stod(val, &idx);

        if (idx == val.length())
        {
            return result;
        }
        else
        {
            throw std::invalid_argument("CsvReader::stringToDouble: cannot convert string to double");
        }
    }
    catch(const std::exception& e)
    {
        throw;
    }
    
}

unsigned int CsvReader::stringToUnsignedInt(std::string const& val)
{
    unsigned int result {};
    size_t idx {};

    try
    {
        result = std::stoi(val, &idx);

        if (idx == val.length())
        {
            return result;
        }
        else
        {
            throw std::invalid_argument("cannot convert string to unsigned integer successfully");
        }
    }
    catch(const std::exception& e)
    {
        throw;
    }
    
}

std::tuple<double, double> CsvReader::getPriceAndAmount(std::string const& priceString, 
                                                        std::string const& amountString)
{
    try
    {
        double price  = CsvReader::stringToDouble(priceString);
        double amount = CsvReader::stringToDouble(amountString);

        return std::make_tuple(price, amount);
    }
    catch(const std::exception& e)
    {
        std::cout << "CsvReader::getPriceAndAmount: Problem converting string data to OrderBookEntry type. " 
                    << "Inputs: " << std::quoted(priceString) << " and " 
                    << std::quoted(amountString) << " "
                    << e.what() << std::endl;
        throw;
    }
}
