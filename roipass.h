#pragma once

#include "ipass.h"

/**
 * @class ROIPass
 *
 * Classe permettant de définir une zone d'intérêt sur une image
 */
class ROIPass : public IPass
{
protected:
    /**
     * Coordonnée en abscisse de la zone d'intérêt
     */
    int xOffset;

    /**
     * Coordonnée en ordonnée de la zone d'intérêt
     */
    int yOffset;

    /**
     * Largeur de la zone d'intérêt
     *
     * Lorsque width < 0, la zone est considérée comme invalide
     */
    int width;

    /**
     * Hauteur de la zone d'intérêt
     *
     * Lorsque height < 0, la zone est considérée comme invalide
     */
    int height;

public:
    /**
     * Constructeur
     *
     * @param xOffset La coordonnée en abscisse de la zone d'intérêt
     * @param yOffset La coordonnée en ordonnée de la zone d'intérêt
     * @param width   La largeur de la zone d'intérêt, une valeur négative invalide la zone
     * @param height  La hauteur de la zone d'intérêt, une valeur négative invalide la zone
     */
    explicit ROIPass(int xOffset = 0, int yOffset = 0, int width = -1, int height = -1);

    /**
     * Destructeur
     */
    virtual ~ROIPass();

    /**
     * Accesseur : xOffset
     *
     * @return La coordonnée en abscisse de la zone d'intérêt
     */
    int getXOffset() const;

    /**
     * Accesseur : xOffset
     *
     * @param xOffset La coordonnée en abscisse de la zone d'intérêt
     */
    void setXOffset(int xOffset);

    /**
     * Accesseur : yOffset
     *
     * @return La coordonnée en ordonnée de la zone d'intérêt
     */
    int getYOffset() const;

    /**
     * Accesseur : yOffset
     *
     * @param La coordonnée en ordonnée de la zone d'intérêt
     */
    void setYOffset(int yOffset);

    /**
     * Accesseur : width
     *
     * @return La largeur de la zone d'intérêt
     */
    int getWidth() const;

    /**
     * Accesseur : width
     *
     * @param width La largeur de la zone d'intérêt, une valeur négative invalide la zone
     */
    void setWidth(int width);

    /**
     * Accesseur : height
     *
     * @return La hauteur de la zone d'intérêt
     */
    int getHeight() const;

    /**
     * Accesseur : height
     *
     * @param height La hauteur de la zone d'intérêt, une valeur négative invalide la zone
     */
    void setHeight(int height);

    /**
     * Changement de tous les paramètres
     *
     * @param xOffset La coordonnée en abscisse de la zone d'intérêt
     * @param yOffset La coordonnée en ordonnée de la zone d'intérêt
     * @param width   La largeur de la zone d'intérêt, une valeur négative invalide la zone
     * @param height  La hauteur de la zone d'intérêt, une valeur négative invalide la zone
     */
    void setParameters(int xOffset, int yOffset, int width, int height);

    /**
     * Invalide la zone d'intérêt
     */
    void clear();

    /**
     * Implémentation : IPass
     */
    IplImage *operator()(IplImage *source);
};
