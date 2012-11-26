#pragma once

#include "ipass.h"

/**
 * @class BlurPass
 *
 * Lisse une image
 */
class BlurPass : public IPass
{
protected:
    /**
     * Algorithme de lissage
     */
    int method;

    /**
     * Premier paramètre de l'algorithme
     */
    int param1;

    /**
     * Second paramètre de l'algorithme
     */
    int param2;

    /**
     * Troisième paramètre de l'algorithme
     */
    double param3;

    /**
     * Quatrième paramètre de l'algorithme
     */
    double param4;

public:
    /**
     * Constructeur
     *
     * @param method L'algorithme de lissage
     * @param param1 Le premier paramètre de l'algorithme
     * @param param2 Le second paramètre de l'algorithme
     * @param param3 Le troisième paramètre de l'algorithme
     * @param param4 Le quatrième paramètre de l'algorithme
     */
    explicit BlurPass(int    method = CV_BLUR_NO_SCALE,
                      int    param1 = 11,
                      int    param2 = 11,
                      double param3 = 0,
                      double param4 = 0);

    /**
     * Destructeur
     */
    virtual ~BlurPass();

    /**
     * Accesseur : method
     *
     * @return L'algorithme de lissage
     */
    int getMethod() const;

    /**
     * Accesseur : method
     *
     * @param method L'algorithme de lissage
     */
    void setMethod(int method);

    /**
     * Accesseur : param1
     *
     * @return Le premier paramètre de l'algorithme
     */
    int getParam1() const;

    /**
     * Accesseur : param1
     *
     * @param param1 Le premier paramètre de l'algorithme
     */
    void setParam1(int param1);

    /**
     * Accesseur : param2
     *
     * @return param2 Le second paramètre de l'algorithme
     */
    int getParam2() const;

    /**
     * Accesseur : param2
     *
     * @param Le second paramètre de l'algorithme
     */
    void setParam2(int param2);

    /**
     * Accesseur : param3
     *
     * @return Le troisième paramètre de l'algorithme
     */
    double getParam3() const;

    /**
     * Accesseur : param3
     *
     * @param param3 Le troisième paramètre de l'algorithme
     */
    void setParam3(double param3);

    /**
     * Accesseur : param4
     *
     * @return Le quatrième paramètre de l'algorithme
     */
    double getParam4() const;

    /**
     * Accesseur : param4
     *
     * @param param4 Le quatrième paramètre de l'algorithme
     */
    void setParam4(double param4);

    /**
     * Changement de tous les paramètres
     *
     * @param method L'algorithme de lissage utilisé
     * @param param1 Le premier paramètre de l'algorithme
     * @param param2 Le second paramètre de l'algorithme
     * @param param3 Le troisième paramètre de l'algorithme
     * @param param4 Le quatrième paramètre de l'algorithme
     */
    void setParameters(int method, int param1, int param2, double param3, double param4);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);
};
