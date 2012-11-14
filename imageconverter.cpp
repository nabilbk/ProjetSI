#include "imageconverter.h"

#include <QDebug>

QImage ImageConverter::IplImageToQImage(const IplImage *source)
{
    int width = source->width;
    int height = source->height;

    QImage result = QImage(width, height, QImage::Format_ARGB32);
    char *data = source->imageData;

    for (int y = 0; y < height; ++y, data += source->widthStep)
    {
        for (int x = 0; x < width; ++x)
        {
            if (source->nChannels == 3)
            {
                int baseOffset = x * source->nChannels;

                result.setPixel(x, y, qRgb(data[baseOffset + 2],
                                           data[baseOffset + 1],
                                           data[baseOffset + 0]));
            }
            else
                result.setPixel(x, y, qRgb(data[x], data[x], data[x]));
        }
    }

    return result;
}

QPixmap ImageConverter::IplImageToQPixmap(const IplImage *source)
{
    return QPixmap::fromImage(ImageConverter::IplImageToQImage(source));
}

IplImage *ImageConverter::QImageToIplImage(const QImage *source)
{
    // Dans le cas d'une image en noir et blanc
    if (source->format() <= 3)
    {
        CvSize size = cvSize(source->width(), source->height());

        IplImage *result = cvCreateImage(size, IPL_DEPTH_8U, 1);
        char *data = (char*)result->imageData;

        for (int y = 0; y < size.height; ++y, data += result->widthStep)
            for (int x = 0; x < size.width; ++x)
            {
                data[x] = (char)qGray(source->pixel(x, y));
            }

        return result;
    }

    // Pour la couleur
    IplImage* cvImage;
    IplImage* colorImage;

    cvImage = cvCreateImageHeader(cvSize(source->width(), source->height()), IPL_DEPTH_8U, 4);
    cvImage->imageData = (char*)source->bits();
    colorImage = cvCreateImage(cvGetSize(cvImage), 8, 3);
    cvConvertImage(cvImage, colorImage, 0);

    cvReleaseImage(&cvImage);

    return colorImage;
}
