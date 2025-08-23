NAME = WebServer
CC = c++
# CPPFLAGS =  -g   -Wall -Wextra -Werror 
SRC = $(wildcard *.cpp) $(wildcard configParsing/*.cpp) $(wildcard CoreServer/src/*.cpp) \
		$(wildcard ./ParseRequest/*.cpp) $(wildcard ./response/*.cpp)
HEADER = $(wildcard *.hpp) $(wildcard configParsing/*.hpp) $(wildcard ./ParseRequest/*.hpp) $(wildcard ./response/*.hpp)
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