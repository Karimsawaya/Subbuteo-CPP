CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = subuteo
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

re: clean all