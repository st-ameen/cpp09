#include "BitcoinExchange.hpp"

int main(int ac, char **av) {

    if (ac != 2)
    {
        std::cout << "Usage: ./btc filename" << std::endl;
        return 1;
    }

    try {
        BitcoinExchange btc;

        btc.loadDatabase();
        btc.processInput(av[1]);
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
