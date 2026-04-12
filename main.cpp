#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>

#include "Jeu.hpp"
#include "Settings.hpp"

int main() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(Settings::SCREEN_WIDTH),
                       static_cast<unsigned int>(Settings::SCREEN_HEIGHT)}),
        "Subuteo Evolution");
    window.setFramerateLimit(static_cast<unsigned int>(Settings::FPS));

    Jeu jeu;
    Objet* joueurSelectionne = nullptr;

    // Chargement de la texture du fond
    sf::Texture textureHerbe;
    if (!textureHerbe.loadFromFile("grass.jpg"))
        std::cerr << "Erreur chargement : grass.jpg" << std::endl;

    sf::Sprite spriteFond(textureHerbe);
    if (textureHerbe.getSize().x > 0 && textureHerbe.getSize().y > 0) {
        spriteFond.setScale({
            static_cast<float>(Settings::SCREEN_WIDTH)  / static_cast<float>(textureHerbe.getSize().x),
            static_cast<float>(Settings::SCREEN_HEIGHT) / static_cast<float>(textureHerbe.getSize().y)
        });
    }

    // Chargement de la police
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
        std::cerr << "Erreur chargement : arial.ttf" << std::endl;

    // Formes réutilisables pour le rendu
    sf::CircleShape formeBalle(static_cast<float>(jeu.terrain.balle.rayon) * Settings::SCALE);
    formeBalle.setFillColor(Settings::COLOR_BALL);
    formeBalle.setOrigin({formeBalle.getRadius(), formeBalle.getRadius()});

    sf::CircleShape formeJoueur(static_cast<float>(jeu.terrain.rouges[0].rayon) * Settings::SCALE);
    formeJoueur.setOrigin({formeJoueur.getRadius(), formeJoueur.getRadius()});
    formeJoueur.setOutlineThickness(2.0f);

    // Texte du score affiché en haut
    sf::Text texteScore(font);
    texteScore.setCharacterSize(30);
    texteScore.setFillColor(sf::Color::White);
    texteScore.setOutlineColor(sf::Color::Black);
    texteScore.setOutlineThickness(2.0f);
    texteScore.setPosition({10.0f, 8.0f});

    // Texte "BUT !" affiché au centre après un but
    sf::Text texteBut(font);
    texteBut.setCharacterSize(80);
    texteBut.setFillColor(sf::Color::Yellow);
    texteBut.setOutlineColor(sf::Color::Black);
    texteBut.setOutlineThickness(4.0f);
    texteBut.setString("BUT !");

    // Indicateur du tour en cours (bas de l'écran)
    sf::Text texteTour(font);
    texteTour.setCharacterSize(22);
    texteTour.setOutlineColor(sf::Color::Black);
    texteTour.setOutlineThickness(2.0f);
    texteTour.setPosition({10.0f, static_cast<float>(Settings::SCREEN_HEIGHT) - 30.0f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Les actions de tir ne sont possibles que pendant ATTENTE_TIR
            if (jeu.etat == GameState::ATTENTE_TIR) {

                // Clic gauche : sélectionner un joueur de l'équipe active
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                        auto& equipe = (jeu.tourActuel == CouleurEquipe::ROUGE)
                                       ? jeu.terrain.rouges
                                       : jeu.terrain.bleus;
                        for (int i = 0; i < 5; i++) {
                            Vecteur2D p = equipe[i].position * Settings::SCALE;
                            if (std::hypot(p.x - mousePos.x, p.y - mousePos.y)
                                < equipe[i].rayon * Settings::SCALE) {
                                joueurSelectionne = &equipe[i];
                            }
                        }
                    }
                }

                // Relâcher le clic : tir en slingshot (direction opposée au glisser)
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
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

        jeu.update();

        // --- Rendu ---
        window.clear();
        window.draw(spriteFond);

        // Ligne de visée entre le joueur sélectionné et la souris
        if (joueurSelectionne) {
            sf::Vector2f start = {
                static_cast<float>(joueurSelectionne->position.x) * Settings::SCALE,
                static_cast<float>(joueurSelectionne->position.y) * Settings::SCALE
            };
            sf::Vector2f end = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            sf::Vertex line[] = {
                {.position = start, .color = Settings::COLOR_VISEE},
                {.position = end,   .color = sf::Color::Transparent}
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        // Dessin de la balle
        formeBalle.setPosition({
            static_cast<float>(jeu.terrain.balle.position.x) * Settings::SCALE,
            static_cast<float>(jeu.terrain.balle.position.y) * Settings::SCALE
        });
        window.draw(formeBalle);

        // Dessin des joueurs
        for (int i = 0; i < 5; i++) {
            auto dessinerJoueur = [&](Joueur& joueur, sf::Color couleur) {
                formeJoueur.setFillColor(couleur);

                // Contour jaune si ce joueur est sélectionné, sinon contour noir
                bool estSelectionne = (joueurSelectionne == &joueur);
                formeJoueur.setOutlineColor(estSelectionne ? Settings::COLOR_SELECTION : sf::Color::Black);
                formeJoueur.setOutlineThickness(estSelectionne ? 4.0f : 2.0f);

                formeJoueur.setPosition({
                    static_cast<float>(joueur.position.x) * Settings::SCALE,
                    static_cast<float>(joueur.position.y) * Settings::SCALE
                });
                window.draw(formeJoueur);
            };

            dessinerJoueur(jeu.terrain.rouges[i], Settings::COLOR_ROUGE);
            dessinerJoueur(jeu.terrain.bleus[i],  Settings::COLOR_BLEU);
        }

        // Score
        texteScore.setString(
            Settings::Player1 + ": " + std::to_string(jeu.scoreRouge) +
            "  -  " +
            Settings::Player2 + ": " + std::to_string(jeu.scoreBleu)
        );
        window.draw(texteScore);

        // Indicateur du tour
        std::string tourStr = "Tour : ";
        tourStr += (jeu.tourActuel == CouleurEquipe::ROUGE) ? Settings::Player1 : Settings::Player2;
        texteTour.setFillColor(jeu.tourActuel == CouleurEquipe::ROUGE
                               ? Settings::COLOR_ROUGE
                               : Settings::COLOR_BLEU);
        texteTour.setString(tourStr);
        window.draw(texteTour);

        // Affichage du message "BUT !" pendant l'état BUT_MARQUE
        if (jeu.etat == GameState::BUT_MARQUE) {
            sf::FloatRect bounds = texteBut.getLocalBounds();
            texteBut.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            texteBut.setPosition({
                static_cast<float>(Settings::SCREEN_WIDTH)  / 2.0f,
                static_cast<float>(Settings::SCREEN_HEIGHT) / 2.0f
            });
            window.draw(texteBut);
        }

        window.display();
    }

    return 0;
}
