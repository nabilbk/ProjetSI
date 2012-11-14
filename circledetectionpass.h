#pragma once

#include "circle.h"
#include "ipass.h"

using namespace std;

/**
 * @class CircleDetectionPass
 *
 * Fournit une interface permettant de détecter des cercles sur une image
 */
class CircleDetectionPass : public IPass
{
protected:
    /**
     * Cercles détectés
     */
    list<Circle> circles;

public:
    /**
     * Destructeur
     */
    virtual ~CircleDetectionPass();

    /**
     * Accesseur : circles
     *
     * @return Les cercles détectés
     */
    virtual list<Circle> getCircles() const;

    /**
     * Vide la liste des cercles détectés
     */
    void clear();
};
