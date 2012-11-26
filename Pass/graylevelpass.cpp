#include "graylevelpass.h"

GrayLevelPass::~GrayLevelPass()
{
}

IplImage *GrayLevelPass::operator()(IplImage *source)
{
    // Image déjà en niveaux de gris
    if (source->nChannels == 1)
        return source;

    IplImage *result = cvCreateImage(cvGetSize(source), 8, 1);

    cvCvtColor(source, result, CV_BGR2GRAY);
    cvReleaseImage(&source);

    return result;
}
