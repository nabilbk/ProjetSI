#pragma once

#include "ipass.h"

/**
 * @class BinaryConversionPass
 *
 * Permet de binariser une image
 */
class BinaryConversionPass : public IPass
{
protected:
    /**
     * Algorithme de seuillage utilisé
     */
    int mode;

    /**
     * Seuil de binarisation
     */
    double threshold;

    /**
     * Valeur maximum de binarisation
     */
    double maxValue;

public:
    /**
     * Constructeur
     *
     * @param mode      L'algorithme de seuillage utilisé
     * @param threshold Le seuil de binarisation
     * @param maxValue  La valeur maximum de binarisation
     */
    explicit BinaryConversionPass(int    mode      = CV_THRESH_BINARY,
                                  double threshold = 100,
                                  double maxValue  = 255);

    /**
     * Destructeur
     */
    virtual ~BinaryConversionPass();

    /**
     * Accesseur : mode
     *
     * @return L'algorithme de seuillage utilisé
     */
    int getMode() const;

    /**
     * Accesseur : mode
     *
     * @param mode L'algorithme de seuillage utilisé
     */
    void setMode(int mode);

    /**
     * Accesseur : threshold
     *
     * @return Le seuil de binarisation
     */
    double getThreshold() const;

    /**
     * Accesseur : threshold
     *
     * @param threshold Seuil de binarisation
     */
    void setThreshold(double threshold);

    /**
     * Accesseur : maxValue
     *
     * @return La valeur maximum de binarisation
     */
    double getMaxValue() const;

    /**
     * Accesseur : maxValue
     *
     * @param maxValue La valeur maximum de binarisation
     */
    void setMaxValue(double maxValue);

    /**
     * Changement de tous les paramètres
     *
     * @param mode      L'algorithme de seuillage utilisé
     * @param threshold Le seuil de binarisation
     * @param maxValue  La valeur maximum de binarisation
     */
    void setParameters(int mode, double threshold, double maxValue);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};
