#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <iterator>


class PmergeMe
{
    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void    parseInput(int argc, char **argv);
        void    run();

        /* ponytail: exposed to public so the anonymous namespace template in .cpp can access it */
        std::vector<size_t> buildJacobsthalInsertionOrder(size_t pendCount);

    private:
        std::vector<unsigned int>   _originalVector;
        std::deque<unsigned int>    _originalDeque;

        std::vector<unsigned int>   sortVector(std::vector<unsigned int> vec);
        std::deque<unsigned int>    sortDeque(std::deque<unsigned int> deq);

        static bool         isValidNumberToken(const std::string &token);
        static unsigned int tokenToUnsignedInt(const std::string &token);
};

class InvalidInputException : public std::exception
{
    public:
        virtual const char  *what() const throw();
};
