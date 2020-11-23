#include "csession.h"

// Construstors -----------------------------------------------------------------------------------
CSession::CSession()
{
    this->_file = "";
    this->_tabNumber = -1;
}

CSession::CSession(int tabNumber, QString name, QString file, QList<HightLight> lst)
{
    this->_tabNumber = tabNumber;
    this->_file = file;
    this->_name = name;

    this->_lstHighLight.clear();

    // Add the new highlight word.
    for(int i = 0; i < lst.count(); i++)
    {
        HightLight elt = lst.at(i);
        this->_lstHighLight.push_back(elt);
    }
}

CSession::CSession(QString str)
{
    QStringList sLst = str.split('|');

    this->_tabNumber = sLst.at(0).toInt();
    this->_file = sLst.at(1);

    if(sLst.count() > 2)
    {
        QStringList sHiglights = sLst.at(2).split(',');
        QString lStr;

        foreach(lStr, sHiglights)
        {
            this->_lstHighLight.push_back(HightLight(lStr, false));
        }
    }
}

// Getters / Setters ------------------------------------------------------------------------------
QString CSession::GetFile()
{
    return this->_file;
}

int CSession::GetTabNumber()
{
    return this->_tabNumber;
}

QList<HightLight> CSession::GetHighLight()
{
    return this->_lstHighLight;
}

QString CSession::GetName()
{
    return this->_name;
}

void CSession::SetName(QString name)
{
    this->_name = name;
}

void CSession::SetHighLight(QList<HightLight> lst)
{
    this->_lstHighLight.clear();

    // Add the new highlight word.
    for(int i = 0; i < lst.count(); i++)
    {
        HightLight elt = lst.at(i);
        this->_lstHighLight.push_back(elt);
    }
}

void CSession::SetFile(QString file)
{
    this->_file = file;
}

void CSession::SetTabNumber(int tabNumber)
{
    this->_tabNumber = tabNumber;
}

// Public Methods ---------------------------------------------------------------------------------
QString CSession::ToString()
{
    QString sRet;

    sRet.setNum(this->_tabNumber);
    sRet += "|";
    sRet += this->_file;
    sRet += "|";

    for(int i = 0; i < this->_lstHighLight.count(); i++)
    {
        HightLight elt = this->_lstHighLight.at(i);

        sRet += elt.ToString();
        sRet += ",";
    }

    // Remove the last ',' character.
    return sRet.remove(sRet.size() - 1, 1);
}
