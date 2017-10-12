#include "ztreeitemmodel.h"

ZTreeItemModel::ZTreeItemModel()
{

}

ZTreeItemModel::~ZTreeItemModel()
{

}

bool ZTreeItemModel::hasIcon() const
{
    return mHasIcon;
}

void ZTreeItemModel::setHasIcon(bool hasIcon)
{
    mHasIcon = hasIcon;
}

QVariant ZTreeItemModel::value() const
{
    return mValue;
}

void ZTreeItemModel::setValue(const QVariant &value)
{
    mValue = value;
}

QColor ZTreeItemModel::color() const
{
    return mColor;
}

void ZTreeItemModel::setColor(const QColor &color)
{
    mColor = color;
}

QIcon ZTreeItemModel::icon() const
{
    return mIcon;
}

void ZTreeItemModel::setIcon(const QIcon &icon)
{
    mIcon = icon;
}
