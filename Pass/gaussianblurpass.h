#pragma once

#include "blurpass.h"

/**
 * @class GaussianBlurPass
 *
 * Lisse une image en utilisant un flou gaussien
 */
class GaussianBlurPass : public BlurPass
{
public:
    /**
     * Constructeur
     *
     * @param radius La taille du noyau de convolution
     */
    explicit GaussianBlurPass(int radius = 3);

    /**
     * Destructeur
     */
    virtual ~GaussianBlurPass();

    /**
     * Accesseur : radius
     *
     * @return La taille du noyau de convolution
     */
    int getRadius() const;

    /**
     * Accesseur : radius
     *
     * @param radius La taille du noyau de convolution
     */
    void setRadius(int radius);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};

