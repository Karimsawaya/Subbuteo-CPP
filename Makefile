# Nom du programme final
NAME = subbuteo

# Compilateur (g++ pour le C++)
CXX = g++

# Options de compilation : -Wall (tous les avertissements), -std=c++17 (standard moderne)
CXXFLAGS = -Wall -Wextra -std=c++17

# Chemins pour SFML 3 sur Mac (Homebrew)
SFML_INC = -I/opt/homebrew/include
SFML_LIB = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

# Liste des fichiers source
SRCS = main.cpp

# Liste des fichiers objets (générés automatiquement à partir des .cpp)
OBJS = $(SRCS:.cpp=.o)

# Règle principale
all: $(NAME)

# Compilation du binaire final
$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(SFML_LIB)

# Compilation des fichiers .o à partir des .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INC) -c $< -o $@

# Nettoyage des fichiers temporaires
clean:
	rm -f $(OBJS)

# Nettoyage complet
fclean: clean
	rm -f $(NAME)

# Re-compiler de zéro
re: fclean all
