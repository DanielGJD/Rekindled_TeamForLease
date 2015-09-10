COMPILER=g++
FLAGS=-Wall -Wextra -Werror -ansi -pedantic
SRC=src/
BIN=bin/
EXE=$(BIN)Game


Engine.o: $(SRC)Engine.cpp $(SRC)Engine.h
	$(COMPILER) -c $(FLAGS) $(SRC)Engine.cpp -o $(BIN)Engine.o

clean:
	rm -rf $(BIN)*