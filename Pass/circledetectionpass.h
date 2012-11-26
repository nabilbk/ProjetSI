#pragma once

#include "circle.h"
#include "ipass.h"

/**
 * @class CircleDetectionPass
 * Classe Virtuelle pure permettant de détecter des cercles sur une image. toto
    @see CustomCircleDetectionPass
    @see HoughCircleDetectionPass
 */
class CircleDetectionPass : public IPass
{
protected:
    /** @brief Cercles détectés lors du traitement de l'image.
     */
    IPass::ListCircle circles;

public:
    /**
     * Destructeur
     */
    virtual ~CircleDetectionPass();

    /**
        @brief Retourne les cercles détectes lors du traitement de l'image
        @return ListCircle : liste des cercles trouvés
     */
    virtual IPass::ListCircle getCircles() const;

    /**
     @brief Vide la liste des cercles détectés.
     */
    void clear(void);
};
