#pragma once

#include "circle.h"
#include "ipass.h"

/**
 * @class CircleDetectionPass
 * Classe Virtuelle pure permettant de d�tecter des cercles sur une image. toto
    @see CustomCircleDetectionPass
    @see HoughCircleDetectionPass
 */
class CircleDetectionPass : public IPass
{
protected:
    /** @brief Cercles d�tect�s lors du traitement de l'image.
     */
    IPass::ListCircle circles;

public:
    /**
     * Destructeur
     */
    virtual ~CircleDetectionPass();

    /**
        @brief Retourne les cercles d�tectes lors du traitement de l'image
        @return ListCircle : liste des cercles trouv�s
     */
    virtual IPass::ListCircle getCircles() const;

    /**
     @brief Vide la liste des cercles d�tect�s.
     */
    void clear(void);
};
