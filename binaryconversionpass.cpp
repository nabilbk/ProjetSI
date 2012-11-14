#include "binaryconversionpass.h"

#include <stdexcept>

using namespace std;

BinaryConversionPass::BinaryConversionPass(int mode, double threshold, double maxValue)
{
    setParameters(mode, threshold, maxValue);
}

BinaryConversionPass::~BinaryConversionPass()
{
}

int BinaryConversionPass::getMode() const
{
    return mode;
}

void BinaryConversionPass::setMode(int mode)
{
    if (mode != CV_THRESH_BINARY && mode != CV_THRESH_BINARY_INV && mode != CV_THRESH_TRUNC &&
        mode != CV_THRESH_TOZERO && mode != CV_THRESH_TOZERO_INV && mode != CV_THRESH_MASK  &&
        mode != CV_THRESH_OTSU)
        throw invalid_argument("Invalid argument : mode");

    this->mode = mode;
}

double BinaryConversionPass::getThreshold() const
{
    return threshold;
}

void BinaryConversionPass::setThreshold(double threshold)
{
    if (threshold < 0 || threshold > 255)
        throw invalid_argument("Invalid argument : threshold");

    this->threshold = threshold;
}

double BinaryConversionPass::getMaxValue() const
{
    return maxValue;
}

void BinaryConversionPass::setMaxValue(double maxValue)
{
    if (maxValue < 0 || maxValue > 255)
        throw invalid_argument("Invalid argument : maxValue");

    this->maxValue = maxValue;
}

void BinaryConversionPass::setParameters(int mode, double threshold, double maxValue)
{
    setMode(mode);
    setThreshold(threshold);
    setMaxValue(maxValue);
}

IplImage *BinaryConversionPass::operator()(IplImage *source)
{
    cvThreshold(source, source, threshold, maxValue, mode);

    return source;
}
