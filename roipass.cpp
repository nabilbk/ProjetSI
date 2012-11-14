#include "roipass.h"

ROIPass::ROIPass(int xOffset, int yOffset, int width, int height)
{
    setParameters(xOffset, yOffset, width, height);
}

ROIPass::~ROIPass()
{
}

int ROIPass::getXOffset() const
{
    return xOffset;
}

void ROIPass::setXOffset(int xOffset)
{
    this->xOffset = xOffset;
}

int ROIPass::getYOffset() const
{
    return yOffset;
}

void ROIPass::setYOffset(int yOffset)
{
    this->yOffset = yOffset;
}

int ROIPass::getWidth() const
{
    return width;
}

void ROIPass::setWidth(int width)
{
    this->width = width;
}

int ROIPass::getHeight() const
{
    return height;
}

void ROIPass::setHeight(int height)
{
    this->height = height;
}

void ROIPass::setParameters(int xOffset, int yOffset, int width, int height)
{
    setXOffset(xOffset);
    setYOffset(yOffset);
    setWidth(width);
    setHeight(height);
}

void ROIPass::clear()
{
    setWidth(-1);
    setHeight(-1);
}

IplImage *ROIPass::operator()(IplImage *source)
{
    if (width < 0 || height < 0)
        cvResetImageROI(source);
    else
        cvSetImageROI(source, cvRect(xOffset, yOffset, width, height));

    return source;
}
