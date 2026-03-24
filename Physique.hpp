#ifndef PHYSIQUE_HPP
#define PHYSIQUE_HPP

#include "Objets.hpp"
#include <cmath>

class Physique {
public:

    // Frottement visqueux : v = v * (1 - coeff * dt), avec amortissement
    void frottement(Objet& obj, double coeff, double dt = 0.1) {
        if (dt <= 0 || coeff <= 0) return;

        double factor = std::max(0.0, 1.0 - coeff * dt);
        obj.vitesse = obj.vitesse.multiplier_scalaire(factor);

        if (obj.vitesse.norme() <= 0.01) {
            obj.vitesse = Vecteur2D(0, 0);
        }
    }

    // Distance au carré (évite sqrt inutile). Utile pour tests de collision rapides.
    double distance2(const Objet& o1, const Objet& o2) const {
        Vecteur2D d = o2.position.soustraction_vecteur(o1.position);
        return d.norme2();
    }

    bool collision(const Objet& o1, const Objet& o2) const {
        double r = o1.rayon + o2.rayon;
        return distance2(o1, o2) <= r * r;
    }

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

    // Rebond sur les murs avec restitution < 1 pour perte d'énergie
    void gerer_murs(Objet& obj, double largeur, double hauteur, double restitution = 0.8) {
        if (restitution < 0.0) restitution = 0.0;
        if (restitution > 1.0) restitution = 1.0;

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

    void gerer_collision(Objet& o1, Objet& o2, double restitution) {
        if (!collision(o1, o2)) return;

        corriger_penetration(o1, o2);

        Vecteur2D N = o2.position.soustraction_vecteur(o1.position);
        double d = N.norme();
        if (d <= 0.0) return;

        N = N.normaliser();

        double v1n = o1.vitesse.produit_scalaire(N);
        double v2n = o2.vitesse.produit_scalaire(N);

        double rel = v1n - v2n;
        if (rel <= 0.0) return;

        double m1 = o1.masse;
        double m2 = o2.masse;

        double impulse = (-(1.0 + restitution) * rel) / (1.0/m1 + 1.0/m2);

        Vecteur2D impulseVec = N.multiplier_scalaire(impulse);

        o1.vitesse = o1.vitesse.addition_vecteur(impulseVec.multiplier_scalaire(1.0 / m1));
        o2.vitesse = o2.vitesse.soustraction_vecteur(impulseVec.multiplier_scalaire(1.0 / m2));
    }
};

#endif