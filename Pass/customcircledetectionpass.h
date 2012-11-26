#pragma once

#include "circledetectionpass.h"

#include <vector>

using namespace std;

/**
 * @class CustomCircleDetectionPass
 *
 * Permet de d�tecter des ensembles de points (�ventuellement des cercles) sur une image,
 * en utilisant notre m�thode perso
 */
class CustomCircleDetectionPass : public CircleDetectionPass
{
protected:
    /**
     * Tableau de bits repr�sentant les pixels de l'image
     *
     * Un �l�ment est true s'il appartient au "vide" de l'image, false s'il appartient � un "cercle"
     */
    vector<bool>  pixels;

    /**
     * Tableau de shorts indiquant l'appartenance de chaque pixel � un groupe
     * false signifie que le pixel n'appartient � aucun groupe
     */
    vector<bool> pixelGroups;

    /**
     * Largeur de la zone d'int�r�t
     */
    int width;

    /**
     * Hauteur de la zone d'int�r�t
     */
    int height;

    /**
     * Offset en x de la zone d'int�r�t
     */
    int xOffset;

    /**
     * Offset en y de la zone d'int�r�t
     */
    int yOffset;

    /**
     * Nombre minimum de points sur les cercles d�tect�s
     */
    int minPoints;

    /**
     * Rayon minimum des cercles d�tect�s
     */
    double minRadius;

    /**
     * Rayon maximum des cercles d�tect�s
     */
    double maxRadius;

    /**
     * Mode de calcul, true pour automatique, false pour calcul � partir d'un clic souris
     */
    bool autoDetect;

    /**
     * D�termine si un point de l'image n'a pas encore �t� parcouru
     *
     * @param point Le point
     * @return true si le point n'a pas encore �t� parcouru, false sinon
     */
    bool nonWalked(pair<int, int> point) const;

    /**
     * D�termine si un point de l'image est un point de contour
     *
     * @param point Le point
     * @return true si le point est un point de contour, false sinon
     */
    bool isEdge(pair<int, int> point) const;

    /**
     * D�termine si un cercle a �t� d�tect� partiellement
     *
     * @param c Le cercle
     * @return true si le cercle est en intersection avec les bornes de la zone d'int�r�t,
     *         false sinon
     */
    bool isPartial(Circle c) const;

    /**
     * D�tecte les cercles automatiquement sur la zone d'int�r�t sp�cifi�e
     */
    void doAutoDetection();

    /**
     * Tente de d�tecter un cercle en partant de (xOffset; yOffset)
     */
    void doManualDetection();

public:
    /**
     * Constructeur
     *
     * @param minPoints  Nombre de points minimum des cercles
     * @param minRadius  Rayon minimal des cercles
     * @param maxRadius  Rayon maximal des cercles
     * @param autoDetect Mode de d�tection
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
     * @return Le mode de d�tection
     */
    bool getAutoDetect() const;

    /**
     * Accesseur : autoDetect
     *
     * @param autoDetect Le mode de d�tection
     */
    void setAutoDetect(bool autoDetect);

    /**
     * Changement de tous les param�tres
     *
     * @param minPoints  Nombre de points minimum des cercles
     * @param minRadius  Rayon minimal des cercles
     * @param maxRadius  Rayon maximal des cercles
     * @param autoDetect Mode de d�tection
     */
    void setParameters(int minPoints, double minRadius, double maxRadius, bool autoDetect);

    /**
     * Utilis� en mode manuel pour d�finir le point de d�part de l'algorithme
     *
     * @param xOffset La coordonn�e en abscisse du point de d�part
     * @param yOffset La coordonn�e en ordonn�e du point de d�part
     */
    void setOffset(int xOffset, int yOffset);

    /**
     * Impl�mentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};
