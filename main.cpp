
#include <iostream>
#include "Vecteur2D.hpp"
#include "Objets.hpp"
#include "Physique.hpp"

int main(){

    Objet obj1(0,0,2,0,1,1);
    Objet obj2(8,1,0,0,1,1);

    Physique physique;

    double dt = 0.1;

    for(int i=0;i<10;i++){

        std::cout<<"Iteration "<<i<<std::endl;

        obj1.deplacer(dt);
        obj2.deplacer(dt);

        physique.frottement(obj1,0.0);
        physique.frottement(obj2,0.0);

        if(physique.collision(obj1,obj2)){

            std::cout<<"Collision detectee"<<std::endl;

            physique.gerer_collision(obj1,obj2);
        }

        std::cout<<"Obj1 position ";
        obj1.afficher_position();

        std::cout<<"Obj1 vitesse ";
        obj1.afficher_vitesse();

        std::cout<<"Obj2 position ";
        obj2.afficher_position();

        std::cout<<"Obj2 vitesse ";
        obj2.afficher_vitesse();

        std::cout<<"----------------"<<std::endl;
    }

    return 0;
}