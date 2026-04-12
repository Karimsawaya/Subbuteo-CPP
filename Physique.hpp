#ifndef PHYSIQUE_HPP
#define PHYSIQUE_HPP

#include "Objets.hpp"
#include <cmath>

class Physique {
public:

    // Frottement visqueux : réduit la vitesse à chaque frame
    void frottement(Objet& obj, double coeff, double dt = 0.1) {
        if (dt <= 0 || coeff <= 0) return;
        double factor = std::max(0.0, 1.0 - coeff * dt);
        obj.vitesse = obj.vitesse.multiplier_scalaire(factor);
        if (obj.vitesse.norme() <= 0.01)
            obj.vitesse = Vecteur2D(0, 0);
    }

    // Distance au carré entre deux objets (évite un sqrt inutile)
    double distance2(const Objet& o1, const Objet& o2) const {
        return o2.position.soustraction_vecteur(o1.position).norme2();
    }

    // Retourne vrai si les deux objets se chevauchent
    bool collision(const Objet& o1, const Objet& o2) const {
        double r = o1.rayon + o2.rayon;
        return distance2(o1, o2) <= r * r;
    }

    // Sépare deux objets qui se chevauchent en les poussant chacun de la moitié de la pénétration
    void corriger_penetration(Objet& o1, Objet& o2) {
        Vecteur2D N = o2.position.soustraction_vecteur(o1.position);
        double d = N.norme();
        if (d <= 0.0) return;

        double penetration = o1.rayon + o2.rayon - d;
        if (penetration <= 0.0) return;

        N = N.normaliser();
        Vecteur2D correction = N.multiplier_scalaire(penetration / 2.0);
        o1.position = o1.position.soustraction_vecteur(correction);
        o2.position = o2.position.addition_vecteur(correction);
    }

    // Rebond sur les quatre murs pour les joueurs (pas de passage dans les buts)
    void gerer_murs(Objet& obj, double largeur, double hauteur, double restitution = 0.8) {
        restitution = std::max(0.0, std::min(1.0, restitution));

        if (obj.position.x - obj.rayon < 0) {
            obj.position.x = obj.rayon;
            obj.vitesse.x = -obj.vitesse.x * restitution;
        } else if (obj.position.x + obj.rayon > largeur) {
            obj.position.x = largeur - obj.rayon;
            obj.vitesse.x = -obj.vitesse.x * restitution;
        }

        if (obj.position.y - obj.rayon < 0) {
            obj.position.y = obj.rayon;
            obj.vitesse.y = -obj.vitesse.y * restitution;
        } else if (obj.position.y + obj.rayon > hauteur) {
            obj.position.y = hauteur - obj.rayon;
            obj.vitesse.y = -obj.vitesse.y * restitution;
        }
    }

    // Version spéciale pour la balle : laisse passer dans l'ouverture des buts (gauche et droite)
    // Les murs haut et bas restent toujours actifs
    void gerer_murs_avec_buts(Objet& obj, double largeur, double hauteur, double restitution,
                               double butYHaut, double butYBas) {
        restitution = std::max(0.0, std::min(1.0, restitution));

        // Murs haut et bas : toujours actifs
        if (obj.position.y - obj.rayon < 0) {
            obj.position.y = obj.rayon;
            obj.vitesse.y = -obj.vitesse.y * restitution;
        } else if (obj.position.y + obj.rayon > hauteur) {
            obj.position.y = hauteur - obj.rayon;
            obj.vitesse.y = -obj.vitesse.y * restitution;
        }

        // Mur gauche : rebond sauf si la balle est dans l'ouverture du but
        if (obj.position.x - obj.rayon < 0) {
            bool dansOuverture = (obj.position.y > butYHaut && obj.position.y < butYBas);
            if (!dansOuverture) {
                obj.position.x = obj.rayon;
                obj.vitesse.x = -obj.vitesse.x * restitution;
            }
            // Sinon la balle entre dans le but, verifierBut() s'en charge
        }

        // Mur droit : idem
        if (obj.position.x + obj.rayon > largeur) {
            bool dansOuverture = (obj.position.y > butYHaut && obj.position.y < butYBas);
            if (!dansOuverture) {
                obj.position.x = largeur - obj.rayon;
                obj.vitesse.x = -obj.vitesse.x * restitution;
            }
        }
    }

    // Résolution de collision élastique par impulsion entre deux objets
    void gerer_collision(Objet& o1, Objet& o2, double restitution) {
        if (!collision(o1, o2)) return;

        corriger_penetration(o1, o2);

        Vecteur2D N = o2.position.soustraction_vecteur(o1.position);
        double d = N.norme();
        if (d <= 0.0) return;

        N = N.normaliser();

        // Vitesses relatives projetées sur l'axe de collision
        double v1n = o1.vitesse.produit_scalaire(N);
        double v2n = o2.vitesse.produit_scalaire(N);
        double rel = v1n - v2n;

        // Si les objets s'éloignent déjà, pas besoin de résoudre
        if (rel <= 0.0) return;

        double m1 = o1.masse;
        double m2 = o2.masse;
        double impulse = (-(1.0 + restitution) * rel) / (1.0 / m1 + 1.0 / m2);

        Vecteur2D impulseVec = N.multiplier_scalaire(impulse);
        o1.vitesse = o1.vitesse.addition_vecteur(impulseVec.multiplier_scalaire(1.0 / m1));
        o2.vitesse = o2.vitesse.soustraction_vecteur(impulseVec.multiplier_scalaire(1.0 / m2));
    }
};

#endif
