#ifndef MEANSHIFTFILTERPASS_H
#define MEANSHIFTFILTERPASS_H

#include "ipass.h"

#include <QObject>

/**
  * @class MeanShiftFilterPass
  *
  * Applique l'algorithme meanshift a l'image
  */
class MeanShiftFilterPass : public QObject, public IPass
{
    Q_OBJECT
public:
    /**
      * Constructeur
      */
    MeanShiftFilterPass(int level = 3,
                        double threshold1 = 200,
                        double threshold2 = 50);

    /**
     * Changement de tous les paramètres
     *
     * @param threshold1   Le seuil 1
     * @param threshold2   Le seuil 2
     * @param level Niveau maximum de la segmentation pyramidale
     */
    void setParameters(int level = 3,
                       double threshold1 = 200,
                       double threshold2 = 50,
                       bool forceSegmentation = false);

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);

signals:
    void error(QString err);

private:
    int level;
    double threshold1;
    double threshold2;
    bool forceSegmentation;
};

#endif // MEANSHIFTFILTERPASS_H
