#ifndef OBJETS_HPP
#define OBJETS_HPP

#include "Vecteur2D.hpp"
#include "Settings.hpp"
#include <iostream>

// Classe de base pour tout objet physique du jeu (balle, joueur)
class Objet {
public:
    Vecteur2D position;
    Vecteur2D vitesse;
    Vecteur2D acceleration;
    double masse;
    double rayon;

    Objet() : position(0, 0), vitesse(0, 0), acceleration(0, 0), masse(1), rayon(1) {}

    Objet(double x, double y, double vx, double vy, double m, double r)
        : position(x, y), vitesse(vx, vy), acceleration(0, 0), masse(m), rayon(r) {}

    // Intégration semi-implicite d'Euler : vitesse puis position
    void deplacer(double dt, Vecteur2D acc = Vecteur2D(0, 0)) {
        vitesse += acc * dt;
        position += vitesse * dt;
    }

    void afficher_position() const { position.afficher(); }
    void afficher_vitesse()  const { vitesse.afficher(); }
};

// La balle : plus légère et plus petite que les joueurs
class Balle : public Objet {
public:
    Balle() : Objet(
        Settings::TERRAIN_LARGEUR / 2.0,  // Spawn au centre du terrain
        Settings::TERRAIN_HAUTEUR / 2.0,
        0, 0, 0.5, 0.3
    ) {}
};

enum CouleurEquipe { ROUGE, BLEU };

// Un joueur sur le terrain
class Joueur : public Objet {
public:
    int id;
    CouleurEquipe equipe;

    Joueur() : Objet(0, 0, 0, 0, 1, 0.6), id(0), equipe(ROUGE) {}

    Joueur(double x, double y, int id_, CouleurEquipe eq)
        : Objet(x, y, 0, 0, 1, 0.6), id(id_), equipe(eq) {}

    void afficher() {
        std::cout << "Joueur " << id << " ";
        std::cout << (equipe == ROUGE ? "Rouge" : "Bleu") << std::endl;
        std::cout << "Position : ";
        position.afficher();
    }
};

// Le terrain contient la balle et les deux équipes
class Terrain {
public:
    double largeur;
    double hauteur;
    Balle balle;
    Joueur rouges[5];
    Joueur bleus[5];

    Terrain() {
        // Dimensions tirées de Settings pour éviter les valeurs en dur
        largeur = Settings::TERRAIN_LARGEUR;
        hauteur = Settings::TERRAIN_HAUTEUR;
        initialiser_joueurs();
    }

    void initialiser_joueurs() {
        for (int i = 0; i < 5; i++) {
            // Équipe rouge sur la gauche, bleue sur la droite
            rouges[i] = Joueur(3,            2 + i * 1.5, i, ROUGE);
            bleus[i]  = Joueur(largeur - 3,  2 + i * 1.5, i, BLEU);
        }
    }
};

#endif
