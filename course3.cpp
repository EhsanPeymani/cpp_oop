#include <string>
#include <iostream>
#include "ReadCsv.h"
#include "CSVReader.h"


int main()
{
    std::string filename = "data.csv";
    ReadCsv csv {filename};

    if (!csv.readFile())
    {
        std::cout << "Failed in reading CSV" << std::endl;
        return 1;
    }

    auto const& objects = csv.getObjects();

    unsigned int count{};
    for (auto const& obj : objects)
    {
        std::cout << "Data Line: " << ++count << " - " 
                    << obj.timestamp << ", " << obj.product << ", " << obj.price 
                    << ", " << obj.amount << std::endl;
    }


    std::cout << "-----------------------------" << std::endl;
    auto output = CsvReader::readCsv(filename);
    
    return 0;
}