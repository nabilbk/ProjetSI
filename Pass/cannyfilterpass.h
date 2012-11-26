#pragma once

#include "ipass.h"

#include <QObject>

/**
 * @class CannyFilterPass
 *
 * Applique le filtre de Canny à une image
 */
class CannyFilterPass: public QObject, public IPass
{
    Q_OBJECT

    /**
     * Premier seuil
     */
    double threshold1;

    /**
     * Second seuil
     */
    double threshold2;

    /**
     * Taille de l'ouverture
     */
    int apertureSize;

public:
    /**
     * Constructeur
     *
     * @param threshold1   Le premier seuil
     * @param threshold2   Le second seuil
     * @param apertureSize La taille de l'ouverture
     */
    explicit CannyFilterPass(double threshold1   = 1,
                             double threshold2   = 20,
                             int    apertureSize = 3);

    /**
     * Destructeur
     */
    virtual ~CannyFilterPass();

    /**
     * Accesseur : threshold1
     *
     * @return Le seuil 1
     */
    double getThreshold1() const;

    /**
     * Accesseur : threshold1
     *
     * @param threshold1 Le seuil 1
     */
    void setThreshold1(double threshold1);

    /**
     * Accesseur : threshold2
     *
     * @return Le seuil 2
     */
    double getThreshold2() const;

    /**
     * Accesseur : threshold2
     *
     * @param threshold2 Le seuil 2
     */
    void setThreshold2(double threshold2);

    /**
     * Accesseur : apertureSize
     *
     * @return La taille de l'ouverture
     */
    int getApertureSize() const;

    /**
     * Accesseur : apertureSize
     *
     * @param apertureSize La taille de l'ouverture
     */
    void setApertureSize(int apertureSize);

    /**
     * Changement de tous les paramètres
     *
     * @param threshold1   Le seuil 1
     * @param threshold2   Le seuil 2
     * @param apertureSize La taille de l'ouverture
     */
    void setParameters(double threshold1    = 1,
                       double threshold2    = 20,
                       int    apertureSize  = 3);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);

signals:
    void error(QString);
};
