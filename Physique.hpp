#ifndef PHYSIQUE_HPP
#define PHYSIQUE_HPP

#include "Objets.hpp"

class Physique{
public:

    void frottement(Objet& obj, double coeff){

        Vecteur2D force = obj.vitesse.multiplier_scalaire(-coeff);
        obj.vitesse = obj.vitesse.addition_vecteur(force);

        if(obj.vitesse.norme() <= 0.1){
            obj.vitesse.x = 0;
            obj.vitesse.y = 0;
        }
    }

    double distance(const Objet& obj1, const Objet& obj2){

        double dx = obj2.position.x - obj1.position.x;
        double dy = obj2.position.y - obj1.position.y;

        return std::sqrt(dx*dx + dy*dy);
    }

    bool collision(const Objet& obj1, const Objet& obj2){

        if(distance(obj1,obj2) <= obj1.rayon + obj2.rayon)
            return true;

        return false;
    }

    void corriger_penetration(Objet& obj1, Objet& obj2){

        Vecteur2D N = obj2.position.soustraction_vecteur(obj1.position);
        double d = N.norme();

        if(d == 0)
            return;

        double penetration = obj1.rayon + obj2.rayon - d;

        if(penetration > 0){

            N = N.normaliser();

            Vecteur2D correction = N.multiplier_scalaire(penetration/2.0);

            obj1.position = obj1.position.soustraction_vecteur(correction);
            obj2.position = obj2.position.addition_vecteur(correction);
        }
    }

    void gerer_collision(Objet& obj1, Objet& obj2){

        if(!collision(obj1,obj2))
            return;

        corriger_penetration(obj1,obj2);

        Vecteur2D N = obj2.position.soustraction_vecteur(obj1.position);
        double d = N.norme();

        if(d == 0)
            return;

        N = N.normaliser();

        double a1 = obj1.vitesse.produit_scalaire(N);
        double a2 = obj2.vitesse.produit_scalaire(N);

        if(a1 - a2 <= 0)
            return;

        Vecteur2D t1 = N.multiplier_scalaire((2*(a1-a2)*obj2.masse)/(obj1.masse+obj2.masse));
        Vecteur2D t2 = N.multiplier_scalaire((2*(a1-a2)*obj1.masse)/(obj1.masse+obj2.masse));

        obj1.vitesse = obj1.vitesse.soustraction_vecteur(t1);
        obj2.vitesse = obj2.vitesse.addition_vecteur(t2);
    }

};

#endif