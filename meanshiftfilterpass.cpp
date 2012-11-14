#include "meanshiftfilterpass.h"

#include <QMessageBox>
#include <QApplication>
#include <QDebug>

MeanShiftFilterPass::MeanShiftFilterPass(int level, double threshold1, double threshold2)
{
    setParameters(level, threshold1, threshold2, false);
}

void MeanShiftFilterPass::setParameters(int level, double threshold1, double threshold2, bool forceSegmentation)
{
    this->level = level;
    this->threshold1 = threshold1;
    this->threshold2 = threshold2;
    this->forceSegmentation = forceSegmentation;
}

int min(int a, int b)
{
    return ((a < b) ? a : b);
}

int timesDivisibleBy2(int size)
{
    int res = 0;

    while (!(size % 2) && size > 1)
    {
        ++res;
        size /= 2;
    }

    return res;
}

int maxLevel(IplImage *img)
{
    return min(timesDivisibleBy2(img->width), timesDivisibleBy2(img->height));
}

IplImage *MeanShiftFilterPass::operator()(IplImage *source)
{
    // Code du traitement
//    cvSmooth(source, source, CV_GAUSSIAN, 15); // Exemple de flou
//    cvThreshold(source, source, 100, 250, CV_THRESH_BINARY); // Exemple de binarisation

    //  Opération de masquage pour que le niveau soit cohérant
    //  The 5th argument (level) cannot be larger than the number of times the smallest dimension of the image is divisible by 2.

    //  ex.
    //  imagesize = 1024x1024
    //  level can be any integer between 1 and 10
    //  1024=2x2x2x2x2x2x2x2x2x2

    if (forceSegmentation)
    {
        source->width &= -(1 << level);
        source->height &= -(1 << level);
    }

    // ici test de la validité du niveau
    if (level > maxLevel(source))
    {
        QString highLevelError("L'image est trop petite pour ce niveau de pyramide\nCette image supporte les niveau 1 - " + QString::number(maxLevel(source)) + ".");
        QString noDivisibleError("L'image n'est pas tolérée par cvPyrSegmentation,\nsa hauteur ou sa largeur n'est pas divisible par 2.\nEssayez avec l'option \"Traitement forcé\".");
        QString err = highLevelError;

        if (!maxLevel(source))
            err = noDivisibleError;

        emit error(err);

//        QApplication::restoreOverrideCursor();
//        QMessageBox::warning(0,
//                             QObject::tr("Erreur"),
//                             err);

        return source;
    }

    IplImage* dst = cvCreateImage(cvGetSize(source), source->depth, source->nChannels);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;

    cvPyrSegmentation(source, dst, storage, &comp, level, threshold1, threshold2);

//    int n_comp = comp->total;
//    for( int i=0; i<n_comp; i++ ) {
//        CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
//        // do_something_with( cc );
//    }

    cvReleaseMemStorage(&storage);
    cvReleaseImage(&source);

    // Image traitée à retourner
    return dst;
}

