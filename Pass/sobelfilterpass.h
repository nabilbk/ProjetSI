#ifndef SOBELFILTERPASS_H
#define SOBELFILTERPASS_H

#include "ipass.h"

#include <QObject>

/**
  * @class SobelFilterPass
  *
  * Applique l'algorithme de d�tection de contours de Sobel a l'image
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
     * Impl�mentation : IPass
     */
    virtual IplImage *operator()(IplImage *source);

signals:
    void error(QString);
};

#endif // SOBELFILTERPASS_H
