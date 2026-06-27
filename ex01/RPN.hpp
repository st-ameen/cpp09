#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cstdlib>


class RPN {
private:
    bool isStrictNumber(const std::string& token, int& outNum) const;

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    int evaluate(const std::string& expression) const;
};
