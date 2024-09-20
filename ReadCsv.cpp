#include "ReadCsv.h"

#include <iostream>
#include <fstream>
#include <exception>

ReadCsv::ReadCsv(std::string _filename)
{
    fileName = _filename;
}

std::vector<std::string> ReadCsv::tokenize(std::string csvLine, char separator)
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

double ReadCsv::stringToDouble(std::string const& val)
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
            throw std::invalid_argument("cannot convert string to double");
        }
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    
}

bool ReadCsv::readFile()
{
    std::ifstream file {fileName};
    std::string line {};

    if (file.is_open())
    {
        std::cout << "file " << fileName << " opened successfully." << std::endl;
        unsigned int count {};

        while (std::getline(file, line))
        {
            std::cout << "Line read " << ++count << std::endl;
            std::vector<std::string> tokens = tokenize(line, ',');

            if (tokens.size() != 5)
            {
                std::cout << "Bad CSV line read. Line " << count << std::endl;
                continue;
            }

            try
            {
                double price  = stringToDouble(tokens[3]);
                double amount = stringToDouble(tokens[4]);

                objects.push_back(OrderBookEntry {tokens[0], tokens[1], tokens[2], price, amount});

            }
            catch(const std::exception& e)
            {
                std::cout << "Problem converting string data to OrderBookEntry type. " 
                            << e.what() << std::endl;
                continue;
            }
        }

        file.close();
        std::cout << "File closed gracefully" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Problem opening the file " << fileName << std::endl;
        return false;
    }
}

std::vector<OrderBookEntry> const& ReadCsv::getObjects()
{
    return objects;
}
