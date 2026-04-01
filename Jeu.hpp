#ifndef JEU_HPP
#define JEU_HPP

#include "Objets.hpp"
#include "Physique.hpp"
#include "Settings.hpp"

enum class GameState { ATTENTE_TIR, EN_MOUVEMENT, BUT_MARQUE };

class Jeu {
public:
    Terrain terrain;
    Physique moteur;
    CouleurEquipe tourActuel;
    GameState etat;
    int scoreRouge;
    int scoreBleu;

    Jeu() : tourActuel(ROUGE), etat(GameState::ATTENTE_TIR), scoreRouge(0), scoreBleu(0) {}

    void verifierBut() {
        double x = terrain.balle.position.x;
        double y = terrain.balle.position.y;

    // Vérification du but à GAUCHE (But pour les Bleus)
        if (x < Settings::BUT_PROFONDEUR) {
        if (y > Settings::BUT_Y_HAUT && y < Settings::BUT_Y_BAS) {
            scoreBleu++;
            reinitialiser();
        } else {
            // Si elle sort mais pas dans le but, c'est juste un rebond ou une sortie
            // On laisse gerer_murs faire son travail ou on remet en jeu
        }
        }

    // Vérification du but à DROITE (But pour les Rouges)
        if (x > terrain.largeur - Settings::BUT_PROFONDEUR) {
            if (y > Settings::BUT_Y_HAUT && y < Settings::BUT_Y_BAS) {
            scoreRouge++;
            reinitialiser();
        }
    }
}

    // Met à jour toute la physique
    void update() {

        
        double dt = Settings::DT;

        verifierBut();

        // 1. Déplacement et friction
        auto maj = [&](Objet& obj) {
            obj.deplacer(dt);
            moteur.frottement(obj, Settings::FRICTION, dt);
            moteur.gerer_murs(obj, terrain.largeur, terrain.hauteur, Settings::RESTITUTION);
        };


        maj(terrain.balle);
        for(int i=0; i<5; i++) {
            maj(terrain.rouges[i]);
            maj(terrain.bleus[i]);
        }

        // 2. Collisions
        for(int i=0; i<5; i++) {
            moteur.gerer_collision(terrain.rouges[i], terrain.balle, Settings::RESTITUTION);
            moteur.gerer_collision(terrain.bleus[i], terrain.balle, Settings::RESTITUTION);
            for(int j=0; j<5; j++) {
                if(i != j) {
                    moteur.gerer_collision(terrain.rouges[i], terrain.rouges[j], Settings::RESTITUTION);
                    moteur.gerer_collision(terrain.bleus[i], terrain.bleus[j], Settings::RESTITUTION);
                }
                moteur.gerer_collision(terrain.rouges[i], terrain.bleus[j], Settings::RESTITUTION);
            }
        }

        // 3. Gestion des règles (Fin de mouvement et Buts)
        if (etat == GameState::EN_MOUVEMENT && tousObjetsArretes()) {
            changerTour();
            etat = GameState::ATTENTE_TIR;
        }


    }

    bool tousObjetsArretes() {
        if (terrain.balle.vitesse.norme() > Settings::SEUIL_ARRET) return false;
        for(int i=0; i<5; i++) {
            if (terrain.rouges[i].vitesse.norme() > Settings::SEUIL_ARRET) return false;
            if (terrain.bleus[i].vitesse.norme() > Settings::SEUIL_ARRET) return false;
        }
        return true;
    }


    void changerTour() {
        tourActuel = (tourActuel == ROUGE) ? BLEU : ROUGE;
    }

    void reinitialiser() {
        terrain = Terrain(); // On recharge un terrain neuf
        etat = GameState::ATTENTE_TIR;
    }



};

#endif