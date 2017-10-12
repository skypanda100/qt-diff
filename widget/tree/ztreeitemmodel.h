#ifndef ZTREEITEMMODEL
#define ZTREEITEMMODEL

#include <QIcon>
#include <QVariant>
#include <QColor>
#include <QMetaType>

class ZTreeItemModel{
public:
    ZTreeItemModel();
    ~ZTreeItemModel();

    bool hasIcon() const;
    void setHasIcon(bool hasIcon);

    QVariant value() const;
    void setValue(const QVariant &value);

    QColor color() const;
    void setColor(const QColor &color);

    QIcon icon() const;
    void setIcon(const QIcon &icon);

private:
    bool mHasIcon;
    QIcon mIcon;
    QVariant mValue;
    QColor mColor;
};

Q_DECLARE_METATYPE(ZTreeItemModel)

#endif // ZTREEITEMMODEL

