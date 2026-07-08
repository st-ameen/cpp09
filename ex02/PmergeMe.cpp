#include "PmergeMe.hpp"

/* ============================================================ */
/* Orthodox Canonical Form                                      */
/* ============================================================ */

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) { *this = other; }

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _originalVector = other._originalVector;
        _originalDeque = other._originalDeque;
    }
    return (*this);
}

PmergeMe::~PmergeMe() {}

const char *InvalidInputException::what() const throw()
{
    return ("Error");
}

/* ============================================================ */
/* Input validation                                             */
/* ============================================================ */

bool PmergeMe::isValidNumberToken(const std::string &token)
{
    if (token.empty()) return (false);
    size_t start = (token[0] == '+') ? 1 : 0;
    if (start == token.size()) return (false);

    for (size_t i = start; i < token.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(token[i])))
            return (false);
    }
    return (true);
}

unsigned int PmergeMe::tokenToUnsignedInt(const std::string &token)
{
    std::istringstream iss(token);
    long value;
    iss >> value;
    if (iss.fail() || !iss.eof() || value < 0 || value > 2147483647L)
        throw (InvalidInputException());
    return (static_cast<unsigned int>(value));
}

void PmergeMe::parseInput(int argc, char **argv)
{
    if (argc < 2) throw (InvalidInputException());

    for (int i = 1; i < argc; ++i)
    {
        std::string token(argv[i]);
        if (!isValidNumberToken(token)) throw (InvalidInputException());

        unsigned int value = tokenToUnsignedInt(token);
        if (std::find(_originalVector.begin(), _originalVector.end(), value) != _originalVector.end())
            throw (InvalidInputException());

        _originalVector.push_back(value);
        _originalDeque.push_back(value);
    }
}

/* ============================================================ */
/* Jacobsthal insertion order                                   */
/* ============================================================ */

std::vector<size_t> PmergeMe::buildJacobsthalInsertionOrder(size_t pendCount)
{
    std::vector<size_t> order;
    if (pendCount == 0) return (order);

    std::vector<bool> used(pendCount, false);
    std::vector<size_t> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);

    while (jacobsthal.back() < pendCount + 2)
    {
        size_t n = jacobsthal.size();
        jacobsthal.push_back(jacobsthal[n - 1] + 2 * jacobsthal[n - 2]);
    }

    size_t prevJ = 1;
    for (size_t k = 2; k < jacobsthal.size(); ++k)
    {
        size_t currJ = std::min(jacobsthal[k], pendCount);
        for (size_t idx = currJ; idx > prevJ; --idx)
        {
            if (idx - 1 < pendCount && !used[idx - 1])
            {
                order.push_back(idx - 1);
                used[idx - 1] = true;
            }
        }
        if (currJ >= pendCount) break;
        prevJ = currJ;
    }

    for (size_t i = 0; i < pendCount; ++i)
    {
        if (!used[i]) order.push_back(i);
    }
    return (order);
}

/* ============================================================ */
/* GENERIC algorithm                                            */
/* ============================================================ */

namespace {
    template <typename Container>
    void insertGeneric(Container &sorted, unsigned int value, size_t upperBound)
    {
        upperBound = std::min(upperBound, sorted.size());
        typename Container::iterator pos = std::lower_bound(
                sorted.begin(), sorted.begin() + upperBound, value);
        sorted.insert(pos, value);
    }

    template <typename Container>
    Container sortGeneric(Container cont, PmergeMe* pm)
    {
        if (cont.size() <= 1) return (cont);

        bool hasStraggler = (cont.size() % 2 == 1);
        unsigned int straggler = 0;
        if (hasStraggler)
        {
            straggler = cont.back();
            cont.pop_back();
        }

        Container winners, losers;
        /* ponytail: reserve() dropped to unify template without SFINAE. Negligible impact for n=3000.
           Add inline template specialization for preallocation if profiling shows vector bottlenecks. */
        for (size_t i = 0; i < cont.size() / 2; ++i)
        {
            unsigned int first = cont[2 * i], second = cont[2 * i + 1];
            losers.push_back(std::min(first, second));
            winners.push_back(std::max(first, second));
        }

        Container mainChain = sortGeneric(winners, pm);
        Container sorted(mainChain.begin(), mainChain.end());

        {
            typename Container::iterator partnerPos = std::find(sorted.begin(), sorted.end(), winners[0]);
            insertGeneric(sorted, losers[0], std::distance(sorted.begin(), partnerPos));
        }

        std::vector<size_t> order = pm->buildJacobsthalInsertionOrder(losers.size() - 1);
        for (size_t i = 0; i < order.size(); ++i)
        {
            size_t idx = order[i] + 1;
            typename Container::iterator partnerPos = std::find(sorted.begin(), sorted.end(), winners[idx]);
            insertGeneric(sorted, losers[idx], std::distance(sorted.begin(), partnerPos));
        }

        if (hasStraggler)
            insertGeneric(sorted, straggler, sorted.size());

        return (sorted);
    }
}

std::vector<unsigned int> PmergeMe::sortVector(std::vector<unsigned int> vec) { return sortGeneric(vec, this); }
std::deque<unsigned int> PmergeMe::sortDeque(std::deque<unsigned int> deq) { return sortGeneric(deq, this); }

/* ============================================================ */
/* run()                                                        */
/* ============================================================ */

static void printSequence(const std::string &label, const std::vector<unsigned int> &seq)
{
    std::cout << label;
    for (size_t i = 0; i < seq.size(); ++i)
        std::cout << seq[i] << (i + 1 < seq.size() ? " " : "");
    std::cout << std::endl;
}

void PmergeMe::run()
{
    printSequence("Before: ", _originalVector);

    std::clock_t startVec = std::clock();
    std::vector<unsigned int> sortedVector = sortVector(_originalVector);
    double vecMicroseconds = static_cast<double>(std::clock() - startVec) * 1000000.0 / CLOCKS_PER_SEC;

    std::clock_t startDeq = std::clock();
    std::deque<unsigned int> sortedDeque = sortDeque(_originalDeque);
    double deqMicroseconds = static_cast<double>(std::clock() - startDeq) * 1000000.0 / CLOCKS_PER_SEC;

    printSequence("After: ", sortedVector);

    std::cout << "Time to process a range of " << _originalVector.size()
        << " elements with std::vector : " << vecMicroseconds << " us\n"
        << "Time to process a range of " << _originalDeque.size()
        << " elements with std::deque : " << deqMicroseconds << " us\n";

    if (sortedVector.size() != sortedDeque.size() ||
        !std::equal(sortedVector.begin(), sortedVector.end(), sortedDeque.begin()))
        std::cerr << "Error" << std::endl;
}
