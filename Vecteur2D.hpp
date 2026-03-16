#ifndef VECTEUR2D_HPP
#define VECTEUR2D_HPP

#include <iostream>
#include <cmath>

class Vecteur2D{

public:

    double x;
    double y;

    Vecteur2D(){
        x = 0;
        y = 0;
    }

    Vecteur2D(double x_, double y_){
        x = x_;
        y = y_;
    }

    Vecteur2D addition_vecteur(const Vecteur2D& v2) const{
        return Vecteur2D(x + v2.x, y + v2.y);
    }

    Vecteur2D addition_scalaire(double k) const{
        return Vecteur2D(x + k, y + k);
    }

    Vecteur2D multiplier_scalaire(double k) const{
        return Vecteur2D(x * k, y * k);
    }

    Vecteur2D soustraction_vecteur(const Vecteur2D& v2) const{
        return Vecteur2D(x - v2.x, y - v2.y);
    }

    Vecteur2D soustraction_scalaire(double k) const{
        return Vecteur2D(x - k, y - k);
    }

    double produit_scalaire(const Vecteur2D& v2) const{
        return x * v2.x + y * v2.y;
    }

    double norme() const{
        return std::sqrt(x * x + y * y);
    }

    double norme2() const{
        return x * x + y * y;
    }

    Vecteur2D normaliser() const{

        double n = norme();

        if(n == 0)
            return Vecteur2D(0,0);

        return Vecteur2D(x / n, y / n);
    }

    void afficher() const{
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }

};

#endif