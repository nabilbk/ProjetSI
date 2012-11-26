#pragma once

#include "circledetectionpass.h"

#include <vector>

using namespace std;

/**
 * @class CustomCircleDetectionPass
 *
 * Permet de détecter des ensembles de points (éventuellement des cercles) sur une image,
 * en utilisant notre méthode perso
 */
class CustomCircleDetectionPass : public CircleDetectionPass
{
protected:
    /**
     * Tableau de bits représentant les pixels de l'image
     *
     * Un élément est true s'il appartient au "vide" de l'image, false s'il appartient à un "cercle"
     */
    vector<bool>  pixels;

    /**
     * Tableau de shorts indiquant l'appartenance de chaque pixel à un groupe
     * false signifie que le pixel n'appartient à aucun groupe
     */
    vector<bool> pixelGroups;

    /**
     * Largeur de la zone d'intérêt
     */
    int width;

    /**
     * Hauteur de la zone d'intérêt
     */
    int height;

    /**
     * Offset en x de la zone d'intérêt
     */
    int xOffset;

    /**
     * Offset en y de la zone d'intérêt
     */
    int yOffset;

    /**
     * Nombre minimum de points sur les cercles détectés
     */
    int minPoints;

    /**
     * Rayon minimum des cercles détectés
     */
    double minRadius;

    /**
     * Rayon maximum des cercles détectés
     */
    double maxRadius;

    /**
     * Mode de calcul, true pour automatique, false pour calcul à partir d'un clic souris
     */
    bool autoDetect;

    /**
     * Détermine si un point de l'image n'a pas encore été parcouru
     *
     * @param point Le point
     * @return true si le point n'a pas encore été parcouru, false sinon
     */
    bool nonWalked(pair<int, int> point) const;

    /**
     * Détermine si un point de l'image est un point de contour
     *
     * @param point Le point
     * @return true si le point est un point de contour, false sinon
     */
    bool isEdge(pair<int, int> point) const;

    /**
     * Détermine si un cercle a été détecté partiellement
     *
     * @param c Le cercle
     * @return true si le cercle est en intersection avec les bornes de la zone d'intérêt,
     *         false sinon
     */
    bool isPartial(Circle c) const;

    /**
     * Détecte les cercles automatiquement sur la zone d'intérêt spécifiée
     */
    void doAutoDetection();

    /**
     * Tente de détecter un cercle en partant de (xOffset; yOffset)
     */
    void doManualDetection();

public:
    /**
     * Constructeur
     *
     * @param minPoints  Nombre de points minimum des cercles
     * @param minRadius  Rayon minimal des cercles
     * @param maxRadius  Rayon maximal des cercles
     * @param autoDetect Mode de détection
     */
    explicit CustomCircleDetectionPass(int minPoints     = 10,
                                       double minRadius  = 3,
                                       double maxRadius  = 200,
                                       bool   autoDetect = true);

    /**
     * Destructeur
     */
    virtual ~CustomCircleDetectionPass();

    /**
     * Accesseur : minPoints
     *
     * @return Le nombre minimum de points des cercles
     */
    int getMinPoints() const;

    /**
     * Accesseur : minPoints
     *
     * @param minPoints Le nombre minimum de points des cercles
     */
    void setMinPoints(int minPoints);

    /**
     * Accesseur : minRadius
     *
     * @return Le rayon minimal des cercles
     */
    double getMinRadius() const;

    /**
     * Accesseur : minRadius
     *
     * @param minRadius Le rayon minimal des cercles
     */
    void setMinRadius(double minRadius);

    /**
     * Accesseur : maxRadius
     *
     * @return Le rayon maximal des cercles
     */
    double getMaxRadius() const;

    /**
     * Accesseur : maxRadius
     *
     * @param maxRadius Le rayon maximal des cercles
     */
    void setMaxRadius(double maxRadius);

    /**
     * Accesseur : autoDetect
     *
     * @return Le mode de détection
     */
    bool getAutoDetect() const;

    /**
     * Accesseur : autoDetect
     *
     * @param autoDetect Le mode de détection
     */
    void setAutoDetect(bool autoDetect);

    /**
     * Changement de tous les paramètres
     *
     * @param minPoints  Nombre de points minimum des cercles
     * @param minRadius  Rayon minimal des cercles
     * @param maxRadius  Rayon maximal des cercles
     * @param autoDetect Mode de détection
     */
    void setParameters(int minPoints, double minRadius, double maxRadius, bool autoDetect);

    /**
     * Utilisé en mode manuel pour définir le point de départ de l'algorithme
     *
     * @param xOffset La coordonnée en abscisse du point de départ
     * @param yOffset La coordonnée en ordonnée du point de départ
     */
    void setOffset(int xOffset, int yOffset);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};
