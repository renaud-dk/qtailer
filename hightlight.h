#ifndef HIGHTLIGHT_H
#define HIGHTLIGHT_H

#include <QString>
#include <QStringList>

class HightLight
{

public:
    explicit HightLight ();
    explicit HightLight (QString strHighLight, QString strColor, bool isDefault = false);
    explicit HightLight (QString strElt, bool isDefault = false);
    void SetHighLightString (QString);
    void SetHighLigthColor (QString);
    bool CompareTo(HightLight elt);
    bool IsDefault();
    QString GetHighLightString ();
    QString GetHighLigthColor ();
    QString ToString();

private:
    bool isDefault;
    QString strHighLight;
    QString strColor;

};

#endif // HIGHTLIGHT_H
