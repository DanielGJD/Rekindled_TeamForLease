COMPILER=g++
FLAGS=-Wall -Wextra -Werror -ansi -pedantic -std=c++11
SRC=src/
BIN=bin/
EXE=$(BIN)Game

# This won't work until we start a test driver
build: $(BIN)test.o $(BIN)Engine.o $(BIN)FrameRateController.o
	$(COMPILER) $(BIN)test.o $(BIN)Engine.o $(BIN)FrameRateController.o -o $(BIN)fle

$(BIN)test.o: $(SRC)__test.cpp
	$(COMPILER) -c $(FLAGS) $(SRC)__test.cpp -o $(BIN)test.o

$(BIN)Engine.o: $(SRC)Engine.cpp $(SRC)Engine.h
	$(COMPILER) -c $(FLAGS) $(SRC)Engine.cpp -o $(BIN)Engine.o

$(BIN)FrameRateController.o: $(SRC)FrameRateController.cpp $(SRC)FrameRateController.h
	$(COMPILER) -c $(FLAGS) $(SRC)FrameRateController.cpp -o $(BIN)FrameRateController.o

clean:
	rm -rf $(BIN)*
