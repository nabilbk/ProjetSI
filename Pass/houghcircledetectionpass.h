#pragma once

#include <opencv2/opencv.hpp>

#include "circledetectionpass.h"

/**
 * @class HoughCircleDetectionPass
 * Traite une image en d�tectant des cercles par la m�thode de Hough.

 */
class HoughCircleDetectionPass : public CircleDetectionPass
{
protected:
    /**
     * R�solution de l'accumulateur
     */
    double dp;

    /**
     * Distance minimale entre deux cercles
     */
    double minDist;

    /**
     * Seuil sup�rieur du filtre de canny
     */
    double param1;

    /**
     * Seuil de l'accumulateur
     */
    double param2;

    /**
     * Rayon minimal des cercles
     */
    int minRad;

    /**
     * Rayon maximal des cercles
     */
    int maxRad;

public:
    /**
      * Constructeur
      *
      * @param dp      R�solution de l'accumulateur
      * @param minDist Distance minimale entre deux cercles
      * @param param1  Seuil sup�rieur du filtre de canny
      * @param param2  Seuil de l'accumulateur
      * @param minRad  Rayon minimal des cercles
      * @param maxRad  Rayon maximal des cercles
      */
    explicit HoughCircleDetectionPass(double dp      = 2,
                             double minDist = 25,
                             double param1  = 50,
                             double param2  = 100,
                             int    minRad  = 1,
                             int    maxRad  = 35);

    /**
     * Destructeur
     */
    virtual ~HoughCircleDetectionPass();

    /**
     * Accesseur : dp
     *
     * @return La r�solution de l'accumulateur
     */
    double getDp() const;

    /**
     * Accesseur : dp
     *
     * @param dp La r�solution de l'accumulateur
     */
    void setDp(double dp);

    /**
     * Accesseur: mindist
     *
     * @return La distance minimale entre deux cercles
     */
    double getMinDist() const;

    /**
     * Accesseur : mindist
     *
     * @param minDist La distance minimale entre deux cercles
     */
    void setMinDist(double minDist);

    /**
     * Accesseur : param1
     *
     * @return Le seuil sup�rieur du filtre de Canny
     */
    double getParam1() const;

    /**
     * Accesseur : param1
     *
     * @param param1 Le seuil sup�rieur de filtre de Canny
     */
    void setParam1(double param1);

    /**
     * Accesseur : param2
     *
     * @return Le seuil de l'accumulateur
     */
    double getParam2() const;

    /**
     * Accesseur : param2
     *
     * @param param2 Le seuil de l'accumulateur
     */
    void setParam2(double param2);

    /**
     * Accesseur : minRad
     *
     * @return Le rayon minimal des cercles
     */
    int getMinRad() const;

    /**
     * Accesseur : minRad
     *
     * @param minRad Le rayon minimal des cercles
     */
    void setMinRad(int minRad);

    /**
     * Accesseur : maxRad
     *
     * @return Le rayon maximal des cercles
     */
    int getMaxRad() const;

    /**
     * Accesseur : maxRad
     *
     * @param maxRad Le rayon maximal des cercles
     */
    void setMaxRad(int maxRad);

    /**
     * Changement de tous les param�tres
     *
     * @param dp      R�solution de l'accumulateur
     * @param minDist Distance minimale entre deux cercles
     * @param param1  Seuil sup�rieur du filtre de canny
     * @param param2  Seuil de l'accumulateur
     * @param minRad  Rayon minimal des cercles
     * @param maxRad  Rayon maximal des cercles
     */
    void setParameters(double dp, double minDist, double param1, double param2, int minRad, int maxRad);

    /** @brief Lance le traitement de l'image.
      Efface les cercles d�tect�s lors du pr�c�dant traitement.
      @param IplImage *source : Image � traiter
      @return IplImage * : image trait�e.
     */
    virtual IplImage *operator()(IplImage *source);
};
