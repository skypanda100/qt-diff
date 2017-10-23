#include "zdiffarea.h"

ZDiffArea::ZDiffArea()
{

}

ZDiffArea::~ZDiffArea()
{

}

float ZDiffArea::startX() const
{
    return mStartX;
}

void ZDiffArea::setStartX(float startX)
{
    mStartX = startX;
}

float ZDiffArea::startY() const
{
    return mStartY;
}

void ZDiffArea::setStartY(float startY)
{
    mStartY = startY;
}

float ZDiffArea::endX() const
{
    return mEndX;
}

void ZDiffArea::setEndX(float endX)
{
    mEndX = endX;
}

float ZDiffArea::endY() const
{
    return mEndY;
}

void ZDiffArea::setEndY(float endY)
{
    mEndY = endY;
}
