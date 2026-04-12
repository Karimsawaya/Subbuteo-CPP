#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Graphics.hpp>

namespace Settings {

    // Noms des joueurs 
    const std::string Player1 = "Leo";
    const std::string Player2 = "Karim";

    // Fenêtre et rendu 
    const int SCREEN_WIDTH  = 800;
    const int SCREEN_HEIGHT = 400;
    const float SCALE = 40.0f;   // 1 mètre physique = 40 pixels
    const float FPS   = 60.0f;
    const float DT    = 1.0f / FPS;

    // Dimensions du terrain (en mètres)
    // Utilisées par Terrain et Balle pour centraliser les valeurs
    const double TERRAIN_LARGEUR = 20.0;
    const double TERRAIN_HAUTEUR = 10.0;

    // Physique
    const double FRICTION       = 0.7;   // Amortissement du mouvement (frottement)
    const double RESTITUTION    = 0.7;   // Coefficient de rebond (murs et collisions)
    const double PUISSANCE_TIR  = 3.0;   // Multiplicateur appliqué à la direction du tir
    const double SEUIL_ARRET    = 0.05;  // Vitesse en dessous de laquelle un objet est considéré immobile
                                    
    // Buts
    // Le terrain fait 20m de large et 10m de haut
    const double BUT_Y_BAS      = 3.8;   // Bord supérieur de l'ouverture du but (en mètres)
    const double BUT_Y_HAUT     = 6.2;   // Bord inférieur de l'ouverture du but
    const double BUT_PROFONDEUR = 0.9;   // Distance depuis le mur pour déclencher un but

    // Durée d'affichage du message "BUT !" avant de réinitialiser le terrain (en secondes)
    const float DUREE_AFFICHAGE_BUT = 2.0f;

    // Couleurs
    const sf::Color COLOR_TERRAIN(34, 139, 34);
    const sf::Color COLOR_BALL(255, 255, 255);
    const sf::Color COLOR_ROUGE(200, 0, 0);
    const sf::Color COLOR_BLEU(0, 0, 200);
    const sf::Color COLOR_VISEE(255, 255, 0, 150);       // Jaune semi-transparent pour la ligne de visée
    const sf::Color COLOR_SELECTION(255, 220, 0, 255);   // Contour jaune vif pour le joueur sélectionné
}

#endif
