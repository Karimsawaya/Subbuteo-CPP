#include <SFML/Graphics.hpp>
#include "Objets.hpp"
#include "Physique.hpp"
#include <optional>
#include <vector>

int main() {
    float scale = 40.f; 
    sf::RenderWindow window(sf::VideoMode({800, 400}), "Subuteo");
    window.setFramerateLimit(60);

    Terrain terrain;
    Physique moteur;
    
    // Pointeur pour suivre quel joueur on est en train de "charger"
    Joueur* joueurSelectionne = nullptr;
    sf::Vertex line[2]; // Pour dessiner la ligne de visée

    // Configuration graphique de la balle
    sf::CircleShape formeBalle(terrain.balle.rayon * scale);
    formeBalle.setFillColor(sf::Color::White);
    formeBalle.setOrigin({(float)terrain.balle.rayon * scale, (float)terrain.balle.rayon * scale});

    // Configuration graphique des joueurs
    sf::CircleShape formeJoueur(terrain.rouges[0].rayon * scale);
    formeJoueur.setOrigin({(float)terrain.rouges[0].rayon * scale, (float)terrain.rouges[0].rayon * scale});
    formeJoueur.setOutlineThickness(2);

    while (window.isOpen()) {
        // --- 1. GESTION DES ÉVÉNEMENTS ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Clic Souris : Sélectionner un joueur
            if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                    
                    // On cherche si on a cliqué sur un joueur rouge
                    for (int i = 0; i < 5; i++) {
                        Vecteur2D p = terrain.rouges[i].position * scale;
                        float dx = p.x - mousePos.x;
                        float dy = p.y - mousePos.y;
                        if (std::sqrt(dx*dx + dy*dy) < terrain.rouges[0].rayon * scale) {
                            joueurSelectionne = &terrain.rouges[i];
                        }
                    }

                    // On cherche si on a cliqué sur un joueur bleu
                    for (int i = 0; i < 5; i++) {
                        Vecteur2D p = terrain.bleus[i].position * scale;
                        float dx = p.x - mousePos.x;
                        float dy = p.y - mousePos.y;
                        if (std::sqrt(dx*dx + dy*dy) < terrain.bleus[0].rayon * scale) {
                            joueurSelectionne = &terrain.bleus[i];
                        }
                    }

                }
            }

            // Relâcher Souris : Propulser le joueur
            if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left && joueurSelectionne) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                    
                    // Calcul du vecteur de propulsion (Position Joueur - Position Souris)
                    Vecteur2D posPhysique = joueurSelectionne->position;
                    Vecteur2D mousePhysique(mousePos.x / scale, mousePos.y / scale);
                    
                    Vecteur2D force = posPhysique.soustraction_vecteur(mousePhysique);
                    joueurSelectionne->vitesse = force.multiplier_scalaire(3.0); // Coefficient de puissance
                    
                    joueurSelectionne = nullptr; // On relâche la sélection
                }
            }
        }

        // --- 2. PHYSIQUE ---
        double dt = 0.016; 

        // Déplacement et friction pour tous
        auto mise_a_jour = [&](Objet& obj) {
            obj.deplacer(dt);
            moteur.frottement(obj, 0.8, dt); // Amortissement
            moteur.gerer_murs(obj, terrain.largeur, terrain.hauteur, 0.7);
        };

        mise_a_jour(terrain.balle);
        for(int i=0; i<5; i++) {
            mise_a_jour(terrain.rouges[i]);
            mise_a_jour(terrain.bleus[i]);
            
            // Collisions Joueurs <-> Balle
            moteur.gerer_collision(terrain.rouges[i], terrain.balle, 0.8);
            moteur.gerer_collision(terrain.bleus[i], terrain.balle, 0.8);

            // Collisions Joueurs <-> Joueurs
            for(int j=0; j<5; j++) {
                moteur.gerer_collision(terrain.rouges[i], terrain.rouges[j], 0.8);
                moteur.gerer_collision(terrain.bleus[i], terrain.bleus[j], 0.8);
                moteur.gerer_collision(terrain.rouges[i], terrain.bleus[j], 0.8);
                moteur.gerer_collision(terrain.bleus[i], terrain.rouges[j], 0.8);
            }   
        }

        // --- 3. RENDU ---
        window.clear(sf::Color(34, 139, 34)); 

        // Dessiner la ligne de visée si on sélectionne un joueur
        if (joueurSelectionne) {
            sf::Vector2f start = {(float)joueurSelectionne->position.x * scale, (float)joueurSelectionne->position.y * scale};
            sf::Vector2f end = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            line[0] = sf::Vertex{.position = start, .color = sf::Color::Yellow};
            line[1] = sf::Vertex{.position = end, .color = sf::Color(255, 255, 0, 128)}; // Semi-transparent yellow
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // Dessiner la balle
        formeBalle.setPosition({(float)terrain.balle.position.x * scale, (float)terrain.balle.position.y * scale});
        window.draw(formeBalle);

        // Dessiner les joueurs
        for (int i = 0; i < 5; i++) {
            // Rouges
            formeJoueur.setFillColor(sf::Color::Red);
            formeJoueur.setPosition({(float)terrain.rouges[i].position.x * scale, (float)terrain.rouges[i].position.y * scale});
            window.draw(formeJoueur);
            // Bleus
            formeJoueur.setFillColor(sf::Color::Blue);
            formeJoueur.setPosition({(float)terrain.bleus[i].position.x * scale, (float)terrain.bleus[i].position.y * scale});
            window.draw(formeJoueur);
        }

        window.display();
    }
    return 0;
}