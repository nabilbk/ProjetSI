#pragma once

#include <list>

using namespace std;

/** @brief Représente un cerle.
 */
struct Circle
{
    /** @brief Abscisse du centre du cercle
     */
    double x;

    /**  @brief Ordonnée du centre du cercle
     */
    double y;

    /** @brief Rayon du cercle
     */
    double r;

    /** @brief
     * Irrégularité du cercle
     *
     * Plus ce nombre est petit, plus le cercle est "parfait"
     */
    double i;

    /**
     * Points de contour du cercle
     */
    list<pair<int, int> > edge;

    /**
     * Constructeur
     *
     * @param x L'abscisse du centre du cercle
     * @param y L'ordonnée du centre du cercle
     * @param r Le rayon du cercle
     */
    explicit Circle(double x = 0.0, double y = 0.0, double r = 0.0);

    /**
     * Constructeur
     *
     * @param edge Les points de contour du cercle
     */
    explicit Circle(list<pair<int, int> > edge);

    /**
     * Destructeur
     */
    virtual ~Circle();
};

/**
 * Opérateur d'égalité
 *
 * @param lhs Opérande de gauche
 * @param rhs Opérande de droite
 *
 * @return True si les deux opérandes sont égales, false sinon
 */
bool operator==(Circle lhs, Circle rhs);
