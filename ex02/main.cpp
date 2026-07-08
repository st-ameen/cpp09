#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	PmergeMe	pmm;

	try
	{
		pmm.parseInput(argc, argv);
		pmm.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
