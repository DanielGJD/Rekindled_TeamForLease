COMPILER=g++
FLAGS=-Wall -Wextra -Werror -ansi -pedantic -std=c++11
SRC=src/
BIN=bin/
EXE=$(BIN)Game

################################################################################

TEST_OBJECTS=$(BIN)test.o $(BIN)Component.o $(BIN)Engine.o $(BIN)Entity.o $(BIN)EntityExceptions.o $(BIN)Exception.o $(BIN)FrameRateController.o $(BIN)GameStateManager.o $(BIN)State.o $(BIN)StateExceptions.o $(BIN)Transform.o
TEST_EXE=$(BIN)fle

$(TEST_EXE): $(TEST_OBJECTS)
	$(COMPILER) $(TEST_OBJECTS) -o $(TEST_EXE)

$(BIN)test.o: $(SRC)__test.cpp
	$(COMPILER) -c $(FLAGS) $(SRC)__test.cpp -o $(BIN)test.o

################################################################################

$(BIN)Component.o: $(SRC)Component.cpp $(SRC)Component.h
	$(COMPILER) -c $(FLAGS) $(SRC)Component.cpp -o $(BIN)Component.o

$(BIN)Engine.o: $(SRC)Engine.cpp $(SRC)Engine.h
	$(COMPILER) -c $(FLAGS) $(SRC)Engine.cpp -o $(BIN)Engine.o

$(BIN)Entity.o: $(SRC)Entity.cpp $(SRC)Entity.h
	$(COMPILER) -c $(FLAGS) $(SRC)Entity.cpp -o $(BIN)Entity.o

$(BIN)EntityExceptions.o: $(SRC)EntityExceptions.cpp $(SRC)EntityExceptions.h
	$(COMPILER) -c $(FLAGS) $(SRC)EntityExceptions.cpp -o $(BIN)EntityExceptions.o

$(BIN)Exception.o: $(SRC)Exception.cpp $(SRC)Exception.h
	$(COMPILER) -c $(FLAGS) $(SRC)Exception.cpp -o $(BIN)Exception.o

$(BIN)FrameRateController.o: $(SRC)FrameRateController.cpp $(SRC)FrameRateController.h
	$(COMPILER) -c $(FLAGS) $(SRC)FrameRateController.cpp -o $(BIN)FrameRateController.o

$(BIN)GameStateManager.o: $(SRC)GameStateManager.cpp $(SRC)GameStateManager.h
	$(COMPILER) -c $(FLAGS) $(SRC)GameStateManager.cpp -o $(BIN)GameStateManager.o

$(BIN)Graphics.o: $(SRC)Graphics.cpp $(SRC)Graphics.h
	$(COMPILER) -c $(FLAGS) $(SRC)Graphics.cpp -o $(BIN)Graphics.o

$(BIN)GraphicsException.o: $(SRC)GraphicsException.cpp $(SRC)GraphicsException.h
	$(COMPILER) -c $(FLAGS) $(SRC)GraphicsException.cpp -o $(BIN)GraphicsException.o

$(BIN)State.o: $(SRC)State.cpp $(SRC)State.h
	$(COMPILER) -c $(FLAGS) $(SRC)State.cpp -o $(BIN)State.o

$(BIN)StateExceptions.o: $(SRC)StateExceptions.cpp $(SRC)StateExceptions.h
	$(COMPILER) -c $(FLAGS) $(SRC)StateExceptions.cpp -o $(BIN)StateExceptions.o

$(BIN)Transform.o: $(SRC)Transform.cpp $(SRC)Transform.h
	$(COMPILER) -c $(FLAGS) $(SRC)Transform.cpp -o $(BIN)Transform.o

clean:
	rm -rf $(BIN)*
