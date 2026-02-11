#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // SFML 3 utilise des accolades {} pour définir la taille de la fenêtre
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Subbuteo Test - SFML 3");
 
    // Création d'un cercle pour représenter un joueur ou la balle
    sf::CircleShape player(30.f);
    player.setFillColor(sf::Color::Red);
    player.setOrigin({30.f, 30.f}); // Centre du cercle
    player.setPosition({400.f, 300.f}); // Milieu de l'écran

    std::cout << "Fenetre ouverte avec succes !" << std::endl;

    // Boucle de jeu (Game Loop)
    while (window.isOpen()) {
        // Gestion des événements (clic, fermeture, etc.)
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 1. On efface l'écran (fond vert pour le terrain)
        window.clear(sf::Color(50, 150, 50));

        // 2. On dessine les objets
        window.draw(player);

        // 3. On affiche le tout
        window.display();
    }

    return 0;
}
