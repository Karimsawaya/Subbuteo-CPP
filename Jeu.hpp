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

    // Met à jour toute la physique
    void update() {
        double dt = Settings::DT;

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

        verifierBut();
    }

    bool tousObjetsArretes() {
        if (terrain.balle.vitesse.norme() > Settings::SEUIL_ARRET) return false;
        for(int i=0; i<5; i++) {
            if (terrain.rouges[i].vitesse.norme() > Settings::SEUIL_ARRET) return false;
            if (terrain.bleus[i].vitesse.norme() > Settings::SEUIL_ARRET) return false;
        }
        return true;
    }

    void verifierBut() {
        // Détection simplifiée sur les bords
        if (terrain.balle.position.x < 0.1) {
            scoreBleu++;
            reinitialiser();
        } else if (terrain.balle.position.x > terrain.largeur - 0.1) {
            scoreRouge++;
            reinitialiser();
        }
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