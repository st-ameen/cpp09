NAME	=	PmergeMe

RM		=	rm -rf
CXX		=   c++
CXXFLAGS	= -I. -Wall -Werror -Wextra -std=c++98

SRC		= main.cpp PmergeMe.cpp

OBJ		=	$(SRC:.cpp=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
			@$(RM) $(OBJ)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
