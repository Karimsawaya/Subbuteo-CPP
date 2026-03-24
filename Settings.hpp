#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Graphics.hpp>

namespace Settings {
    // --- Fenêtre et Rendu ---
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 400;
    const float SCALE = 40.0f;     // 1 mètre physique = 40 pixels
    const float FPS = 60.0f;
    const float DT = 1.0f / FPS;

    // --- Physique ---
    const double FRICTION = 0.8;      // Amortissement des mouvements
    const double RESTITUTION = 0.7;   // Rebond sur les murs/objets
    const double PUISSANCE_TIR = 3.0; // Multiplicateur de force
    const double SEUIL_ARRET = 0.4;  // Vitesse en dessous de laquelle l'objet s'arrête

    // --- Couleurs ---
    const sf::Color COLOR_TERRAIN(34, 139, 34);
    const sf::Color COLOR_BALL(255, 255, 255);
    const sf::Color COLOR_ROUGE(200, 0, 0);
    const sf::Color COLOR_BLEU(0, 0, 200);
    const sf::Color COLOR_VISEE(255, 255, 0, 150); // Jaune semi-transparent

    // --- Règles ---
    const float LARGEUR_BUT = 2.5f; // En mètres
}

#endif