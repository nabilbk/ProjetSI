#pragma once

#include "ipass.h"

/**
 * @class ROIPass
    ROI : Region Of Interest (r�gion d'int�ret).
 *
 * Classe permettant de d�finir une zone d'int�r�t sur une image
 */
class ROIPass : public IPass
{
protected:
    /**
     * Coordonn�e en abscisse de la zone d'int�r�t
     */
    int xOffset;

    /**
     * Coordonn�e en ordonn�e de la zone d'int�r�t
     */
    int yOffset;

    /**
     * Largeur de la zone d'int�r�t
     *
     * Lorsque width < 0, la zone est consid�r�e comme invalide
     */
    int width;

    /**
     * Hauteur de la zone d'int�r�t
     *
     * Lorsque height < 0, la zone est consid�r�e comme invalide
     */
    int height;

public:
    /**
     * Constructeur
     *
     * @param xOffset La coordonn�e en abscisse de la zone d'int�r�t
     * @param yOffset La coordonn�e en ordonn�e de la zone d'int�r�t
     * @param width   La largeur de la zone d'int�r�t, une valeur n�gative invalide la zone
     * @param height  La hauteur de la zone d'int�r�t, une valeur n�gative invalide la zone
     */
    explicit ROIPass(int xOffset = 0, int yOffset = 0, int width = -1, int height = -1);

    /**
     * Destructeur
     */
    virtual ~ROIPass();

    /**
     * Accesseur : xOffset
     *
     * @return La coordonn�e en abscisse de la zone d'int�r�t
     */
    int getXOffset() const;

    /**
     * Accesseur : xOffset
     *
     * @param xOffset La coordonn�e en abscisse de la zone d'int�r�t
     */
    void setXOffset(int xOffset);

    /**
     * Accesseur : yOffset
     *
     * @return La coordonn�e en ordonn�e de la zone d'int�r�t
     */
    int getYOffset() const;

    /**
     * Accesseur : yOffset
     *
     * @param La coordonn�e en ordonn�e de la zone d'int�r�t
     */
    void setYOffset(int yOffset);

    /**
     * Accesseur : width
     *
     * @return La largeur de la zone d'int�r�t
     */
    int getWidth() const;

    /**
     * Accesseur : width
     *
     * @param width La largeur de la zone d'int�r�t, une valeur n�gative invalide la zone
     */
    void setWidth(int width);

    /**
     * Accesseur : height
     *
     * @return La hauteur de la zone d'int�r�t
     */
    int getHeight() const;

    /**
     * Accesseur : height
     *
     * @param height La hauteur de la zone d'int�r�t, une valeur n�gative invalide la zone
     */
    void setHeight(int height);

    /**
     * Changement de tous les param�tres
     *
     * @param xOffset La coordonn�e en abscisse de la zone d'int�r�t
     * @param yOffset La coordonn�e en ordonn�e de la zone d'int�r�t
     * @param width   La largeur de la zone d'int�r�t, une valeur n�gative invalide la zone
     * @param height  La hauteur de la zone d'int�r�t, une valeur n�gative invalide la zone
     */
    void setParameters(int xOffset, int yOffset, int width, int height);

    /**
     * Invalide la zone d'int�r�t
     */
    void clear();

    /**
     * Impl�mentation : IPass
     */
    IplImage *operator()(IplImage *source);
};
