#pragma once

#include <opencv2/opencv.hpp>
#include <list>
#include "circle.h"

/**
 * @class IPass
 * Classe virtuelle pure permettant d'effectuer un traitement sur une image.
   Chaque classe fille définissent un traitement différent possible.
 */
class IPass
{
public:
    /** @brief ListCircle est une liste de cercle où sont stockés les cercles trouvés lors du traitement d'une image */
    typedef list<Circle> ListCircle;

    virtual ~IPass();


    /** @brief Retourne les cercles détectes lors du traitement de l'image
        @return ListCircle : liste des cercles trouvés */
    virtual ListCircle getCircles(void) const;

    /**
     * @brief Lance un traitement sur une image.
     *
     * @param IplImage *source : image à traiter
     * @return IplImage * : image en fin de traitement
     */
    virtual IplImage *operator()(IplImage *source) = 0;
};
