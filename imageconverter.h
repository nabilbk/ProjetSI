#pragma once

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include <QImage>
#include <QPixmap>

/**
 * @class ImageConverter
 *
 * Classe assistante pour convertir des images OpenCV en images Qt, et réciproquement
 */
class ImageConverter
{
public:
    /**
     * Convertit une IplImage en QImage
     *
     * @param source L'IplImage à convertir
     * @return La QImage convertie
     */
    static QImage IplImageToQImage(const IplImage *source);

    /**
     * Convertit une IplImage en QPixmap
     *
     * @param source L'IplImage à convertir
     * @return La QPixmap convertie
     */
    static QPixmap IplImageToQPixmap(const IplImage *source);

    /**
     * Convertit une QImage en QImage
     *
     * @param source La QImage à convertir
     * @return L'IplImage convertie
     */
    static IplImage *QImageToIplImage(const QImage *source);
};
