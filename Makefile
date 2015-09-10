COMPILER=g++
FLAGS=-Wall -Wextra -Werror -ansi -pedantic
SRC=src/
BIN=bin/


ForLeaseEngine.o: $(SRC)ForLeaseEngine.cpp $(SRC)ForLeaseEngine.h
	$(COMPILER) -c $(FLAGS) $(SRC)ForLeaseEngine.cpp -o $(BIN)ForLeaseEngine.o

clean:
	rm -rf $(BIN)*