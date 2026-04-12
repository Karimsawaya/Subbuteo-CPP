#ifndef JEU_HPP
#define JEU_HPP

#include "Objets.hpp"
#include "Physique.hpp"
#include "Settings.hpp"

// États possibles du jeu
enum class GameState {
    ATTENTE_TIR,   // En attente qu'un joueur soit sélectionné et tiré
    EN_MOUVEMENT,  // Objets en mouvement après un tir
    BUT_MARQUE     // Un but vient d'être marqué (affichage temporaire avant reset)
};

class Jeu {
public:
    Terrain terrain;
    Physique moteur;
    CouleurEquipe tourActuel;
    GameState etat;
    int scoreRouge;
    int scoreBleu;
    float tempsAffichageBut; // Compteur pour afficher "BUT !" avant de réinitialiser

    Jeu()
        : tourActuel(ROUGE),
          etat(GameState::ATTENTE_TIR),
          scoreRouge(0),
          scoreBleu(0),
          tempsAffichageBut(0.0f)
    {}

    // Détecte si la balle est entrée dans un des deux buts
    void verifierBut() {
        double x = terrain.balle.position.x;
        double y = terrain.balle.position.y;

        // But à GAUCHE (marque pour les Bleus)
        if (x < Settings::BUT_PROFONDEUR && y > Settings::BUT_Y_BAS && y < Settings::BUT_Y_HAUT) {
            scoreBleu++;
            tourActuel = ROUGE;
            etat = GameState::BUT_MARQUE;
            tempsAffichageBut = Settings::DUREE_AFFICHAGE_BUT;
            return;
        }

        // But à DROITE (marque pour les Rouges)
        if (x > terrain.largeur - Settings::BUT_PROFONDEUR && y > Settings::BUT_Y_BAS && y < Settings::BUT_Y_HAUT) {
            scoreRouge++;
            tourActuel = BLEU;
            etat = GameState::BUT_MARQUE;
            tempsAffichageBut = Settings::DUREE_AFFICHAGE_BUT;
        }
    }

    void update() {
        // Pendant l'affichage du but, on décompte le timer puis on réinitialise
        if (etat == GameState::BUT_MARQUE) {
            tempsAffichageBut -= Settings::DT;
            if (tempsAffichageBut <= 0.0f)
                reinitialiser();
            return;
        }

        double dt = Settings::DT;

        // Déplacement + friction (sans gestion des murs, faite séparément)
        auto maj = [&](Objet& obj) {
            obj.deplacer(dt);
            moteur.frottement(obj, Settings::FRICTION, dt);
        };

        // Balle : murs avec ouverture pour les buts
        maj(terrain.balle);
        moteur.gerer_murs_avec_buts(
            terrain.balle,
            terrain.largeur, terrain.hauteur,
            Settings::RESTITUTION,
            Settings::BUT_Y_HAUT, Settings::BUT_Y_BAS
        );

        // Joueurs : murs pleins (ils ne peuvent pas entrer dans les buts)
        for (int i = 0; i < 5; i++) {
            maj(terrain.rouges[i]);
            maj(terrain.bleus[i]);
            moteur.gerer_murs(terrain.rouges[i], terrain.largeur, terrain.hauteur, Settings::RESTITUTION);
            moteur.gerer_murs(terrain.bleus[i],  terrain.largeur, terrain.hauteur, Settings::RESTITUTION);
        }

        verifierBut();
        if (etat == GameState::BUT_MARQUE) return; // but marqué cette frame, on arrête tout

        // Résolution des collisions
        for (int i = 0; i < 5; i++) {
            moteur.gerer_collision(terrain.rouges[i], terrain.balle, Settings::RESTITUTION);
            moteur.gerer_collision(terrain.bleus[i],  terrain.balle, Settings::RESTITUTION);
            for (int j = 0; j < 5; j++) {
                if (i != j) {
                    moteur.gerer_collision(terrain.rouges[i], terrain.rouges[j], Settings::RESTITUTION);
                    moteur.gerer_collision(terrain.bleus[i],  terrain.bleus[j],  Settings::RESTITUTION);
                }
                moteur.gerer_collision(terrain.rouges[i], terrain.bleus[j], Settings::RESTITUTION);
            }
        }

        // Fin de tour : si tous les objets sont immobiles, on passe à l'équipe suivante
        if (etat == GameState::EN_MOUVEMENT && tousObjetsArretes()) {
            changerTour();
            etat = GameState::ATTENTE_TIR;
        }
    }

    // Vérifie si tous les objets du terrain sont pratiquement immobiles
    bool tousObjetsArretes() {
        if (terrain.balle.vitesse.norme() > Settings::SEUIL_ARRET) return false;
        for (int i = 0; i < 5; i++) {
            if (terrain.rouges[i].vitesse.norme() > Settings::SEUIL_ARRET) return false;
            if (terrain.bleus[i].vitesse.norme()  > Settings::SEUIL_ARRET) return false;
        }
        return true;
    }

    void changerTour() {
        tourActuel = (tourActuel == ROUGE) ? BLEU : ROUGE;
    }

    // Remet le terrain à zéro tout en conservant le score et le tour en cours
    // (tourActuel est défini dans verifierBut() avant l'appel à reinitialiser)
    void reinitialiser() {
        CouleurEquipe tourSauvegarde = tourActuel;
        terrain = Terrain();
        etat = GameState::ATTENTE_TIR;
        tourActuel = tourSauvegarde; // On restaure le tour défini lors du but
    }
};

#endif
