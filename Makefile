NAME = WebServer
CC = c++
<<<<<<< HEAD
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address
SRC = $(wildcard *.cpp) $(wildcard configParsing/*.cpp) $(wildcard CoreServer/src/server/*.cpp) \
		$(wildcard CoreServer/src/logging/*.cpp)
HEADER = $(wildcard *.hpp) $(wildcard configParsing/*.hpp)
=======
CPPFLAGS =  -g -fsanitize=address -DDEBUG_MEMORY  -O0 #-Wall -Wextra -Werror 
SRC = $(wildcard *.cpp) $(wildcard configParsing/*.cpp) $(wildcard CoreServer/src/*.cpp) \
		$(wildcard ./ParseRequest/*.cpp) $(wildcard ./response/*.cpp)
HEADER = $(wildcard *.hpp) $(wildcard configParsing/*.hpp) $(wildcard ./ParseRequest/*.hpp) $(wildcard ./response/*.hpp)
>>>>>>> 7a95435823a8143339d8c12718799f4ad8aa7c87
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