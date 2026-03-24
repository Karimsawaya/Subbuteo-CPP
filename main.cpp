#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include "Jeu.hpp"
#include "Settings.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT}), "Subuteo Evolution");
    window.setFramerateLimit((unsigned int)Settings::FPS);

    Jeu jeu;
    Joueur* joueurSelectionne = nullptr;
    sf::Vertex line[2];

    // Préparation des formes graphiques
    sf::CircleShape formeBalle(jeu.terrain.balle.rayon * Settings::SCALE);
    formeBalle.setFillColor(Settings::COLOR_BALL);
    formeBalle.setOrigin({formeBalle.getRadius(), formeBalle.getRadius()});

    sf::CircleShape formeJoueur(jeu.terrain.rouges[0].rayon * Settings::SCALE);
    formeJoueur.setOrigin({formeJoueur.getRadius(), formeJoueur.getRadius()});
    formeJoueur.setOutlineThickness(2);

    while (window.isOpen()) {
        // 1. INPUTS
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (jeu.etat == GameState::ATTENTE_TIR) {
                if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                        
                        // Chercher quel joueur de l'équipe dont c'est le tour a été cliqué
                        auto& equipe = (jeu.tourActuel == ROUGE) ? jeu.terrain.rouges : jeu.terrain.bleus;
                        for (int i = 0; i < 5; i++) {
                            Vecteur2D p = equipe[i].position * Settings::SCALE;
                            if (std::hypot(p.x - mousePos.x, p.y - mousePos.y) < equipe[i].rayon * Settings::SCALE) {
                                joueurSelectionne = &equipe[i];
                            }
                        }
                    }
                }

                if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left && joueurSelectionne) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                        Vecteur2D mousePhys(mousePos.x / Settings::SCALE, mousePos.y / Settings::SCALE);
                        
                        Vecteur2D dir = joueurSelectionne->position.soustraction_vecteur(mousePhys);
                        joueurSelectionne->vitesse = dir.multiplier_scalaire(Settings::PUISSANCE_TIR);
                        
                        joueurSelectionne = nullptr;
                        jeu.etat = GameState::EN_MOUVEMENT;
                    }
                }
            }
        }

        // 2. LOGIQUE
        jeu.update();

        // 3. RENDU
        window.clear(Settings::COLOR_TERRAIN);

        // Ligne de visée
        if (joueurSelectionne) {
            sf::Vector2f start = {(float)joueurSelectionne->position.x * Settings::SCALE, (float)joueurSelectionne->position.y * Settings::SCALE};
            line[0] = sf::Vertex{.position = start, .color = Settings::COLOR_VISEE};
            line[1] = sf::Vertex{.position = window.mapPixelToCoords(sf::Mouse::getPosition(window)), .color = sf::Color::Transparent};
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // Balle
        formeBalle.setPosition({(float)jeu.terrain.balle.position.x * Settings::SCALE, (float)jeu.terrain.balle.position.y * Settings::SCALE});
        window.draw(formeBalle);

        // Joueurs
        for (int i = 0; i < 5; i++) {
            formeJoueur.setFillColor(Settings::COLOR_ROUGE);
            formeJoueur.setPosition({(float)jeu.terrain.rouges[i].position.x * Settings::SCALE, (float)jeu.terrain.rouges[i].position.y * Settings::SCALE});
            window.draw(formeJoueur);

            formeJoueur.setFillColor(Settings::COLOR_BLEU);
            formeJoueur.setPosition({(float)jeu.terrain.bleus[i].position.x * Settings::SCALE, (float)jeu.terrain.bleus[i].position.y * Settings::SCALE});
            window.draw(formeJoueur);
        }

        window.display();
    }
    return 0;
}