#pragma once

#include <opencv2/opencv.hpp>
#include <list>
#include "circle.h"

/**
 * @class IPass
 * Classe virtuelle pure permettant d'effectuer un traitement sur une image.
   Chaque classe fille d�finissent un traitement diff�rent possible.
 */
class IPass
{
public:
    /** @brief ListCircle est une liste de cercle o� sont stock�s les cercles trouv�s lors du traitement d'une image */
    typedef list<Circle> ListCircle;

    virtual ~IPass();


    /** @brief Retourne les cercles d�tectes lors du traitement de l'image
        @return ListCircle : liste des cercles trouv�s */
    virtual ListCircle getCircles(void) const;

    /**
     * @brief Lance un traitement sur une image.
     *
     * @param IplImage *source : image � traiter
     * @return IplImage * : image en fin de traitement
     */
    virtual IplImage *operator()(IplImage *source) = 0;
};
