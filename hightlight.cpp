#include "hightlight.h"

HightLight::HightLight()
{ }

HightLight::HightLight(QString strElt, bool isDefault)
{
   QStringList lstElt = strElt.split("-");

   this->strHighLight = lstElt[0];
   this->strColor = lstElt[1];
   this->isDefault = isDefault;
}

HightLight::HightLight(QString strHighLight, QString strColor, bool isDefault)
{
    this->strHighLight = strHighLight;
    this->strColor = strColor;
    this->isDefault = isDefault;
}

void HightLight::SetHighLightString (QString strHighLight)
{
    this->strHighLight = strHighLight;
}

void HightLight::SetHighLigthColor(QString strColor)
{
    this->strColor = strColor;
}

QString HightLight::GetHighLightString ()
{
    return this->strHighLight;
}

QString HightLight::GetHighLigthColor ()
{
    return this->strColor;
}

bool HightLight::IsDefault()
{
    return this->isDefault;
}

QString HightLight::ToString()
{
    QString ret = this->strHighLight;
    ret+= "-";
    ret += this->strColor;

    return ret;
}

bool HightLight::CompareTo(HightLight elt)
{
    bool bRet = false;

    if(this->strHighLight == elt.GetHighLightString() &&
       this->strColor == elt.GetHighLigthColor())
    {
        bRet = true;
    }

    return bRet;
}
