#include "RPN.hpp"

RPN::RPN() { }

RPN::RPN(const RPN& other) { *this = other; }

RPN& RPN::operator=(const RPN& other) {
    (void)other;
    return *this;
}

RPN::~RPN() { }

bool RPN::isStrictNumber(const std::string& token, int& outNum) const {
    if (token.empty() || token.length() > 1 || !std::isdigit(token[0])) return false;

    outNum = token[0] - '0';
    return true;
}

int RPN::evaluate(const std::string& expression) const {
    std::stack<int> rpnStack;
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token) {
        if (token.length() == 1 && (token == "+" || token == "-" || token == "*" || token == "/")) {
            
            if (rpnStack.size() < 2) {
                throw std::runtime_error("Error");
            }

            int right = rpnStack.top(); rpnStack.pop();
            int left = rpnStack.top(); rpnStack.pop();

            if (token == "+") rpnStack.push(left + right);
            else if (token == "-") rpnStack.push(left - right);
            else if (token == "*") rpnStack.push(left * right);
            else if (token == "/") {
                if (right == 0) {
                    throw std::runtime_error("Error");
                }
                rpnStack.push(left / right);
            }
        } 
        else {
            int number = 0;
            if (!isStrictNumber(token, number)) {
                throw std::runtime_error("Error");
            }
            rpnStack.push(number);
        }
    }

    if (rpnStack.size() != 1) {
        throw std::runtime_error("Error");
    }

    return rpnStack.top();
}
