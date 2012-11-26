#pragma once

#include "ipass.h"

/**
 * @class GrayLevelPass
 *
 * Permet de convertir une image en niveaux de gris
 */
class GrayLevelPass : public IPass
{
public:
    /**
     * Destructeur
     */
    virtual ~GrayLevelPass();

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};
