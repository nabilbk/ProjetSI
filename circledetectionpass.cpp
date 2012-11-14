#include "circledetectionpass.h"

CircleDetectionPass::~CircleDetectionPass()
{
}

list<Circle> CircleDetectionPass::getCircles() const
{
    return circles;
}

void CircleDetectionPass::clear()
{
    circles.clear();
}
