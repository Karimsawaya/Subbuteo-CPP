TARGET = subuteo
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDE_PATHS = -I/opt/homebrew/include
LIBRARY_PATHS = -L/opt/homebrew/lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Liste de tes headers
DEPS = Vecteur2D.hpp Objets.hpp Physique.hpp Settings.hpp Jeu.hpp

SRC = main.cpp
OBJ = main.o

all: $(TARGET)

# On dit que main.o dépend de main.cpp ET de tous les fichiers .hpp
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBRARY_PATHS) $(LIBS)

clean:
	rm -f $(OBJ) $(TARGET)