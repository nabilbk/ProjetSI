#include "cannyfilterpass.h"

#include <QMessageBox>
#include <QObject>
#include <QApplication>
#include <QIconEngine>

CannyFilterPass::CannyFilterPass(double threshold1, double threshold2, int apertureSize)
{
    setParameters(threshold1, threshold2, apertureSize);
}

CannyFilterPass::~CannyFilterPass()
{
}

double CannyFilterPass::getThreshold1() const
{
    return threshold1;
}

void CannyFilterPass::setThreshold1(double threshold1)
{
    this->threshold1 = threshold1;
}

double CannyFilterPass::getThreshold2() const
{
    return threshold2;
}

void CannyFilterPass::setThreshold2(double threshold2)
{
    this->threshold2 = threshold2;
}

int CannyFilterPass::getApertureSize() const
{
    return apertureSize;
}

void CannyFilterPass::setApertureSize(int apertureSize)
{
    this->apertureSize = apertureSize;
}

void CannyFilterPass::setParameters(double threshold1, double threshold2, int apertureSize)
{
    setThreshold1(threshold1);
    setThreshold2(threshold2);
    setApertureSize(apertureSize);
}

IplImage *CannyFilterPass::operator()(IplImage *source)
{
    // Test du type d'images
    if (source->nChannels != 1)
    {
        emit error(QObject::tr("Le filtre de Canny ne supporte que les image\n") +
                   QObject::tr(" sur canal unique, vous devez donc ajouter\n") +
                   QObject::tr(" \"Niveaux de gris\" en tête de liste\n") +
                   QObject::tr(" avant d'appliquer ce filtre."));
//        QApplication::restoreOverrideCursor();
//        QMessageBox::warning(0,
//                             QObject::tr("Erreur"),
//                             QObject::tr("Le filtre de Canny ne supporte que les image\n") +
//                             QObject::tr(" sur canal unique, vous devez donc ajouter\n") +
//                             QObject::tr(" \"Niveaux de gris\" en tête de liste\n") +
//                             QObject::tr(" avant d'appliquer ce filtre.");

        return source;
    }
    cvCanny(source, source, threshold1, threshold2, apertureSize);

    return source;
}
