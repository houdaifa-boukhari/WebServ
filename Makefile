NAME = WebServer
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
SRC = $(wildcard *.cpp) $(wildcard configParsing/*.cpp) $(wildcard CoreServer/src/*.cpp) \
		$(wildcard CoreServer/src/logging/*.cpp)
HEADER = $(wildcard *.hpp) $(wildcard configParsing/*.hpp)
OBJ = $(SRC:.cpp=.o)
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re