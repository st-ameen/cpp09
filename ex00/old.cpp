#include "BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange() { }

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) { }

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this == &other)
        return *this;

    _data = other._data;

    return *this;
}

BitcoinExchange::~BitcoinExchange() { }


float BitcoinExchange::parse_float(const std::string& n) const {
    std::stringstream ss(n);
    float f = 0.0f;

    ss >> f;
    if (ss.fail())
        throw std::runtime_error("not a valid number => " + n);

    std::string leftover;
    if (ss >> leftover)
        throw std::runtime_error("trailing characters found => " + n);
    return f;
}

bool  BitcoinExchange::isValidDate(const std::string& date) const {
    // todo
}


void BitcoinExchange::loadDatabase() {
    std::ifstream file("data.csv");
    std::string line;

    if (!file.is_open())
        throw std::runtime_error("could not open database.");

    while (std::getline(file, line)) {
        if (line.empty() || line == "date,exchange_rate")
            continue;
    
        std::cout << "Read line: " << line << std::endl;

        std::pair<std::string, float> prex;
        std::size_t pos = line.find(",");
        if (pos == std::string::npos)
            throw std::runtime_error("bad input ==> " + line);

        prex.first = line.substr(0, pos);
        prex.second = parse_float(line.substr(pos + 1));
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
        
    }
}
