#ifndef OBJETS_HPP
#define OBJETS_HPP

#include "Vecteur2D.hpp"
#include <iostream>

class Objet{
    public:

    Vecteur2D position;
    Vecteur2D vitesse;
    Vecteur2D acceleration;
    double masse;
    double rayon;

    Objet() : position(0,0), vitesse(0,0), acceleration(0,0), masse(1), rayon(1) {}

    Objet(double x, double y, double vx, double vy, double m, double r)
        : position(x,y), vitesse(vx,vy), acceleration(0,0), masse(m), rayon(r) {}

    void deplacer(double dt, Vecteur2D acceleration = Vecteur2D(0,0)) {
    // 1. Mise à jour de la vitesse d'abord (Semi-Implicite)
    // On ajoute l'accélération (poussée ou friction) au vecteur vitesse
    vitesse += acceleration * dt;

    // 2. Mise à jour de la position avec la NOUVELLE vitesse
    position += vitesse * dt;
}

    void afficher_position() const{
        position.afficher();
    }

    void afficher_vitesse() const{
        vitesse.afficher();
    }
};

class Balle : public Objet{
public:

    Balle() : Objet(10,5,0,0,0.5,0.3){}
};

enum CouleurEquipe{
    ROUGE,
    BLEU
};

class Joueur : public Objet{
public:

    int id;
    CouleurEquipe equipe;

    Joueur() : Objet(0,0,0,0,1,0.5){
        id = 0;
        equipe = ROUGE;
    }

    Joueur(double x, double y, int id_, CouleurEquipe eq)
        : Objet(x,y,0,0,1,0.5){

        id = id_;
        equipe = eq;
    }

    void afficher(){

        std::cout << "Joueur " << id << " ";

        if(equipe == ROUGE)
            std::cout << "Rouge";
        else
            std::cout << "Bleu";

        std::cout << std::endl;

        std::cout << "Position : ";
        position.afficher();
    }
};

class Terrain{
public:

    double largeur;
    double hauteur;

    Balle balle;

    Joueur rouges[5];
    Joueur bleus[5];

    Terrain(){

        largeur = 20;
        hauteur = 10;

        initialiser_joueurs();
    }

    void initialiser_joueurs(){

        for(int i=0;i<5;i++){

            rouges[i] = Joueur(3, 2 + i*1.5, i, ROUGE);
            bleus[i] = Joueur(17, 2 + i*1.5, i, BLEU);

        }
    }

};

#endif