#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> tokenize(std::string csvLine, char separator)
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
            std::cout   << "start: " << start 
                        << ", end: " << end 
                        << ", Length: " << csvLine.length() << std::endl;
            break;
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


int main()
{
    std::string line = "tt";
    std::vector<std::string> tokens;
    
    std::string filename = "data.csv";
    std::ifstream csvFile {filename};

    if (csvFile.is_open())
    {
        std::cout << "File open" << std::endl;
        
        while(std::getline(csvFile, line))
        {
            std::cout << "Read line " << line << std::endl;
            std::vector<std::string> tokens = tokenize(line, ',');
            for (auto& token : tokens)
            {
                std::cout << token << std::endl;
            }

        }
 
        csvFile.close();
    }
    else
    {
        std::cout << "File Failed to open" << std::endl;
    }



    return 0;
}