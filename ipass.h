#pragma once

#include <opencv/cv.h>
#include <list>
#include "circle.h"

/**
 * @class IPass
 *
 * Définit une interface permettant d'effectuer un traitement sur une image
 */
class IPass
{
public:
    /**
     * Destructeur
     */
    virtual ~IPass();

    virtual list<Circle> getCircles() const;

    /**
     * Exécute le traitement
     *
     * @param source L'image à traiter
     * @return L'image traitée
     */
    virtual IplImage *operator()(IplImage *source) = 0;
};
