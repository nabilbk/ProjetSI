#include "blurpass.h"

#include <stdexcept>
#include <QDebug>

using namespace std;

BlurPass::BlurPass(int method, int param1, int param2, double param3, double param4)
{
    setParameters(method, param1, param2, param3, param4);
}

BlurPass::~BlurPass()
{
}

int BlurPass::getMethod() const
{
    return method;
}

void BlurPass::setMethod(int method)
{
    if (method != CV_BLUR_NO_SCALE && method != CV_BLUR && method != CV_GAUSSIAN &&
        method != CV_MEDIAN        && method != CV_BILATERAL)
        throw invalid_argument("Invalid argument : method");

    this->method = method;
}

int BlurPass::getParam1() const
{
    return param1;
}

void BlurPass::setParam1(int param1)
{
    this->param1 = param1;
}

int BlurPass::getParam2() const
{
    return param2;
}

void BlurPass::setParam2(int param2)
{
    this->param2 = param2;
}

double BlurPass::getParam3() const
{
    return param3;
}

void BlurPass::setParam3(double param3)
{
    this->param3 = param3;
}

double BlurPass::getParam4() const
{
    return param4;
}

void BlurPass::setParam4(double param4)
{
    this->param4 = param4;
}

void BlurPass::setParameters(int method, int param1, int param2, double param3, double param4)
{
    setMethod(method);
    setParam1(param1);
    setParam2(param2);
    setParam3(param3);
    setParam4(param4);
}

IplImage *BlurPass::operator()(IplImage *source)
{
    cvSmooth(source, source, method, param1, param2, param3, param4);

    return source;
}
