#pragma once

#include <iostream>
#include <map>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>


class BitcoinExchange {

    private:
        std::map<std::string, float> _data;

        float   parse_float(const std::string& n) const;
        bool    isValidDate(const std::string& date) const;
        bool    isLeapYear(int year) const;

    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void loadDatabase();
        void processInput(const std::string& filename) const;
};
