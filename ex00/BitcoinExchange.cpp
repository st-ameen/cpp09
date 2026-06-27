#include "BitcoinExchange.hpp"
#include <cstdlib>


BitcoinExchange::BitcoinExchange() { }

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) { }

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this == &other)
        return *this;

    _data = other._data;

    return *this;
}

BitcoinExchange::~BitcoinExchange() { }


double BitcoinExchange::parse_double(const std::string& n) const {
    std::stringstream ss(n);
    double d = 0.0;

    ss >> d;
    if (ss.fail())
        throw std::runtime_error("not a valid number => " + n);

    std::string leftover;
    if (ss >> leftover)
        throw std::runtime_error("trailing characters found => " + n);
    return d;
}

bool BitcoinExchange::isLeapYear(int year) const {
    if (year % 4 != 0)
        return false;
    if (year % 100 != 0)
        return true;
    return (year % 400 == 0);
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    // Expect strict format YYYY-MM-DD (10 chars)
    if (date.size() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }

    int year  = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day   = std::atoi(date.substr(8, 2).c_str());

    if (year < 0)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    static const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year))
        maxDay = 29;

    if (day > maxDay)
        return false;

    return true;
}


void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("could not open database.");

    while (std::getline(file, line)) {
        if (line.empty() || line == "date,exchange_rate")
            continue;

        std::pair<std::string, double> prex;
        std::size_t pos = line.find(",");
        if (pos == std::string::npos)
            continue; // skip malformed db lines instead of aborting the whole load

        prex.first = line.substr(0, pos);
        try {
            prex.second = parse_double(line.substr(pos + 1));
        } catch (std::exception &) {
            continue;
        }
        if (!isValidDate(prex.first))
            continue;

        _data.insert(prex);
    }

    file.close();
    if (_data.empty())
        throw std::runtime_error("file does not contain any data.");
}

void BitcoinExchange::processInput(const std::string& filename) const {
    std::ifstream file(filename.c_str());
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("could not open file ==> " + filename);

    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        if (line == "date | value")
            continue; // skip header

        std::size_t sep = line.find(" | ");
        if (sep == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string dateStr  = line.substr(0, sep);
        std::string valueStr = line.substr(sep + 3);

        if (!isValidDate(dateStr)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        double value;
        try {
            value = parse_double(valueStr);
        } catch (std::exception &) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0) {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000) {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it = _data.lower_bound(dateStr);

        if (it == _data.end()) {
            // dateStr is after every date in db -> use the last (most recent) entry
            --it;
        } else if (it->first != dateStr) {
            // no exact match, lower_bound gave us the first key >= dateStr,
            // we need the closest one that is < dateStr
            if (it == _data.begin()) {
                std::cerr << "Error: no database entry before this date => " << dateStr << std::endl;
                continue;
            }
            --it;
        }
        // else exact match, use it as is

        double result = value * it->second;
        std::cout << dateStr << " => " << value << " = " << result << std::endl;
    }

    file.close();
}