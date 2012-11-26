#include "gaussianblurpass.h"

#include <stdexcept>

using namespace std;

GaussianBlurPass::GaussianBlurPass(int radius)
    : BlurPass(CV_GAUSSIAN)
{
    setRadius(radius);
}

GaussianBlurPass::~GaussianBlurPass()
{
}

int GaussianBlurPass::getRadius() const
{
    return param1;
}

void GaussianBlurPass::setRadius(int radius)
{
    if (radius < 0)
        throw invalid_argument("Invalid argument : radius");

    param1 = radius;
}

IplImage *GaussianBlurPass::operator()(IplImage *source)
{
    cvSmooth(source, source, CV_GAUSSIAN, param1, param2, param3, param4);

    return source;
}
