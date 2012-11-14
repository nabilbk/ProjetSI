#include "houghcircledetectionpass.h"

#include <stdexcept>

HoughCircleDetectionPass::HoughCircleDetectionPass(double dp, double mindist,
                                                   double param1, double param2,
                                                   int minRad, int maxRad)
{
    setParameters(dp, mindist, param1, param2, minRad, maxRad);
}

HoughCircleDetectionPass::~HoughCircleDetectionPass()
{
}

double HoughCircleDetectionPass::getDp() const
{
    return dp;
}

void HoughCircleDetectionPass::setDp(double dp)
{
    if (dp < 1.0)
        throw range_error("Invalid argument : dp");

    this->dp = dp;
}

double HoughCircleDetectionPass::getMinDist() const
{
    return minDist;
}

void HoughCircleDetectionPass::setMinDist(double minDist)
{
    if (minDist < 0)
        throw invalid_argument("Invalid argument : minDist");

    this->minDist = minDist;
}

double HoughCircleDetectionPass::getParam1() const
{
    return param1;
}

void HoughCircleDetectionPass::setParam1(double param1)
{
    if (param1 < 1.0)
        throw invalid_argument("Invalid argument : param1");

    this->param1 = param1;
}

double HoughCircleDetectionPass::getParam2() const
{
    return param2;
}

void HoughCircleDetectionPass::setParam2(double param2)
{
    if (param2 < 0)
        throw invalid_argument("Invalid argument : param2");

    this->param2 = param2;
}

int HoughCircleDetectionPass::getMinRad() const
{
    return minRad;
}

void HoughCircleDetectionPass::setMinRad(int minRad)
{
    if (minRad < 0)
        throw invalid_argument("Invalid argument : minRad");

    this->minRad = minRad;
}

int HoughCircleDetectionPass::getMaxRad() const
{
    return maxRad;
}

void HoughCircleDetectionPass::setMaxRad(int maxRad)
{
    this->maxRad = maxRad;
}

void HoughCircleDetectionPass::setParameters(double dp, double minDist, double param1,
                                             double param2, int minRad, int maxRad)
{
    setDp(dp);
    setMinDist(minDist);
    setParam1(param1);
    setParam2(param2);
    setMinRad(minRad);
    setMaxRad(maxRad);
}

IplImage *HoughCircleDetectionPass::operator()(IplImage *source)
{
    // Suppression des anciens cercles
    circles.clear();

    // Préparation d'une zone de mémoire
    CvMemStorage *storage = cvCreateMemStorage(0);

    CvSeq *cvCircles = cvHoughCircles(source, storage, CV_HOUGH_GRADIENT,
                                      dp, minDist, param1, param2, minRad, maxRad);

    // Stockage des cercles
    for (int i = 0; i < cvCircles->total; ++i)
    {
        float *f = (float*)cvGetSeqElem(cvCircles, i);

        circles.push_back(Circle(f[0], f[1], f[2]));
    }

    cvReleaseMemStorage(&storage);

    return source;
}
