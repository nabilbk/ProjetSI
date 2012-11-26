#ifndef SOBELFILTERPASS_H
#define SOBELFILTERPASS_H

#include "ipass.h"

#include <QObject>

/**
  * @class SobelFilterPass
  *
  * Applique l'algorithme de détection de contours de Sobel a l'image
  */
class SobelFilterPass : public QObject, public IPass
{
    Q_OBJECT
public:
    /**
      * Constructeur
      */
    SobelFilterPass();

    /**
     * Implémentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);

signals:
    void error(QString);
};

#endif // SOBELFILTERPASS_H
