#include <iostream>

class Vecteur2D{

    public:

        double x;
        double y;

        void addition_vecteur(Vecteur2D v2){
            x += v2.x;
            y += v2.y;
        }

        void addition_scalaire(double k){
            x+=k;
            y+=k;
        }

        Vecteur2D multiplier_scalaire(double k){
            Vecteur2D result;
            result.x = x * k;
            result.y = y * k;
            return result;
        }

        void multiplier_vecteur(Vecteur2D v2){
            x*=v2.x;
            y*=v2.y;
        }
        void afficher(){
            std::cout<<"("<<x<<","<<y<<")"<<std::endl;
        }

};

class Objet{
    public:

        Vecteur2D position;
        Vecteur2D vitesse;

        void affecter_position(double x, double y){
            position.x = x;
            position.y = y;
        }
        void affecter_vitesse(double x, double y){
            vitesse.x = x;
            vitesse.y = y;
        }

        void deplacer(int dt){
            vitesse.multiplier_scalaire(dt);
            position.addition_vecteur(vitesse);
        }
        void afficher_position(){
            position.afficher();
        }
        void afficher_vitesse(){
            vitesse.afficher();
        }
};

class Physique{
    public:
    void frottement(Objet obj, double coeff){
        Vecteur2D force_frottement = obj.vitesse.multiplier_scalaire(-coeff);
        obj.vitesse.addition_vecteur(force_frottement);
        if(obj.vitesse.x <= 0.1 && obj.vitesse.y <= 0.1){
            obj.vitesse.x = 0;
            obj.vitesse.y = 0;
        }
    }
};


int main(){
    Physique Physique;
    Objet objet1;
    objet1.affecter_position(0,0);
    objet1.affecter_vitesse(10,5);

    double coef_frottement = 0.05;

    while(objet1.vitesse.x > 0 || objet1.vitesse.y > 0){
        Physique.frottement(objet1, coef_frottement);
        objet1.deplacer(1);
        objet1.afficher_position();
    }

};