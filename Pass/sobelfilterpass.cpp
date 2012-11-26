#include "sobelfilterpass.h"

#include <QApplication>
#include <QMessageBox>

SobelFilterPass::SobelFilterPass()
{
}

IplImage *SobelFilterPass::operator()(IplImage *source)
{
    // Test du type d'images
    if (source->nChannels != 1)
    {
        emit error(QObject::tr("Le filtre de Sobel ne supporte que les image\n") +
                   QObject::tr(" sur canal unique, vous devez donc ajouter\n") +
                   QObject::tr(" \"Niveaux de gris\" en t�te de liste\n") +
                   QObject::tr(" avant d'appliquer ce filtre."));

//        QApplication::restoreOverrideCursor();
//        QMessageBox::warning(0,
//                             QObject::tr("Erreur"),
//                             );

        return source;
    }

    IplImage* filtered = cvCreateImage(cvGetSize(source), IPL_DEPTH_16S, 1);
    IplImage* result = cvCreateImage(cvGetSize(source), IPL_DEPTH_8U, 1);

    // D�tection des contours
    cvSobel(source, filtered, 1, 1, 3);
    // Passage de 16bits Sign�s � 8 bits non sign�s
    cvConvertScaleAbs(filtered , result, 1, 0);

    cvReleaseImage(&source);
    cvReleaseImage(&filtered);

    return result;
}
