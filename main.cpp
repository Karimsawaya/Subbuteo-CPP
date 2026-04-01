#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>

#include "Jeu.hpp"
#include "Settings.hpp"

int main() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(Settings::SCREEN_WIDTH), static_cast<unsigned int>(Settings::SCREEN_HEIGHT)}),
        "Subuteo Evolution");
    window.setFramerateLimit(static_cast<unsigned int>(Settings::FPS));

    Jeu jeu;
    Objet* joueurSelectionne = nullptr;

    sf::Texture textureHerbe;
    if (!textureHerbe.loadFromFile("grass.jpg")) {
        std::cerr << "Erreur : grass.jpg" << std::endl;
    }
    sf::Sprite spriteFond(textureHerbe);
    if (textureHerbe.getSize().x > 0 && textureHerbe.getSize().y > 0) {
        spriteFond.setScale({
            static_cast<float>(Settings::SCREEN_WIDTH) / static_cast<float>(textureHerbe.getSize().x),
            static_cast<float>(Settings::SCREEN_HEIGHT) / static_cast<float>(textureHerbe.getSize().y)});
    }

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Erreur : arial.ttf" << std::endl;
    }

    sf::CircleShape formeBalle(static_cast<float>(jeu.terrain.balle.rayon) * Settings::SCALE);
    formeBalle.setFillColor(Settings::COLOR_BALL);
    formeBalle.setOrigin({formeBalle.getRadius(), formeBalle.getRadius()});

    sf::CircleShape formeJoueur(static_cast<float>(jeu.terrain.rouges[0].rayon) * Settings::SCALE);
    formeJoueur.setOrigin({formeJoueur.getRadius(), formeJoueur.getRadius()});
    formeJoueur.setOutlineThickness(2.0f);

    sf::Text texteScore(font);
    texteScore.setCharacterSize(40);
    texteScore.setFillColor(sf::Color::Black);
    texteScore.setPosition({200.0f, 10.0f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (jeu.etat == GameState::ATTENTE_TIR) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);
                        auto& equipe = (jeu.tourActuel == CouleurEquipe::ROUGE) ? jeu.terrain.rouges : jeu.terrain.bleus;
                        for (int i = 0; i < 5; i++) {
                            Vecteur2D p = equipe[i].position * Settings::SCALE;
                            if (std::hypot(p.x - mousePos.x, p.y - mousePos.y) < equipe[i].rayon * Settings::SCALE) {
                                joueurSelectionne = &equipe[i];
                            }
                        }
                    }
                }

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

        window.clear();
        window.draw(spriteFond);

        if (joueurSelectionne) {
            sf::Vector2f start = {
                static_cast<float>(joueurSelectionne->position.x) * Settings::SCALE,
                static_cast<float>(joueurSelectionne->position.y) * Settings::SCALE};
            sf::Vector2f end = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            sf::Vertex line[] = {
                {.position = start, .color = Settings::COLOR_VISEE},
                {.position = end, .color = sf::Color::Transparent}};
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        formeBalle.setPosition({
            static_cast<float>(jeu.terrain.balle.position.x) * Settings::SCALE,
            static_cast<float>(jeu.terrain.balle.position.y) * Settings::SCALE});
        window.draw(formeBalle);

        for (int i = 0; i < 5; i++) {
            formeJoueur.setFillColor(Settings::COLOR_ROUGE);
            formeJoueur.setPosition({
                static_cast<float>(jeu.terrain.rouges[i].position.x) * Settings::SCALE,
                static_cast<float>(jeu.terrain.rouges[i].position.y) * Settings::SCALE});
            window.draw(formeJoueur);

            formeJoueur.setFillColor(Settings::COLOR_BLEU);
            formeJoueur.setPosition({
                static_cast<float>(jeu.terrain.bleus[i].position.x) * Settings::SCALE,
                static_cast<float>(jeu.terrain.bleus[i].position.y) * Settings::SCALE});
            window.draw(formeJoueur);
        }

        texteScore.setString(
            Settings::Player1 + ": " + std::to_string(jeu.scoreRouge) + " - " + Settings::Player2 + ": " + std::to_string(jeu.scoreBleu));
        window.draw(texteScore);

        window.display();
    }

    return 0;
}
