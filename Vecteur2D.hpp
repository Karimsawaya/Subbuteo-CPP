#ifndef VECTEUR2D_HPP
#define VECTEUR2D_HPP

#include <iostream>
#include <cmath>

// Représente un vecteur 2D (position, vitesse, force...)
class Vecteur2D {
public:
    double x;
    double y;

    Vecteur2D() : x(0), y(0) {}
    Vecteur2D(double x_, double y_) : x(x_), y(y_) {}

    Vecteur2D& operator+=(const Vecteur2D& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vecteur2D operator*(double k) const {
        return Vecteur2D(x * k, y * k);
    }

    Vecteur2D addition_vecteur(const Vecteur2D& v2) const {
        return Vecteur2D(x + v2.x, y + v2.y);
    }

    Vecteur2D addition_scalaire(double k) const {
        return Vecteur2D(x + k, y + k);
    }

    Vecteur2D multiplier_scalaire(double k) const {
        return Vecteur2D(x * k, y * k);
    }

    Vecteur2D soustraction_vecteur(const Vecteur2D& v2) const {
        return Vecteur2D(x - v2.x, y - v2.y);
    }

    Vecteur2D soustraction_scalaire(double k) const {
        return Vecteur2D(x - k, y - k);
    }

    // Produit scalaire : utile pour projeter une vitesse sur un axe (collisions)
    double produit_scalaire(const Vecteur2D& v2) const {
        return x * v2.x + y * v2.y;
    }

    double norme() const {
        return std::sqrt(x * x + y * y);
    }

    // Norme au carré : évite un sqrt, utile pour les tests de distance rapides
    double norme2() const {
        return x * x + y * y;
    }

    // Retourne le vecteur unitaire (même direction, longueur 1)
    Vecteur2D normaliser() const {
        double n = norme();
        if (n == 0) return Vecteur2D(0, 0);
        return Vecteur2D(x / n, y / n);
    }

    void afficher() const {
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }
};

#endif
