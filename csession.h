#ifndef CSESSION_H
#define CSESSION_H

#include <QString>
#include <QList>
#include "hightlight.h"

class CSession
{

public:
    CSession();
    CSession(int tabNumber, QString name, QString file, QList<HightLight> lst);
    CSession(QString str);

    int GetTabNumber();
    QString GetFile();
    QString GetName();
    QList<HightLight> GetHighLight();

    void SetFile (QString file);
    void SetTabNumber(int tabNumber);
    void SetHighLight(QList<HightLight> lst);
    void SetName(QString name);

    QString ToString();

private:
    int _tabNumber;
    QString _file;
    QString _name;
    QList<HightLight> _lstHighLight;

};

#endif // CSESSION_H
