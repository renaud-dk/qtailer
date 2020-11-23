#ifndef QTAILTHREAD_H
#define QTAILTHREAD_H

#include "hightlight.h"

#include <QFile>
#include <QThread>
#include <QSettings>
#include <QDateTime>
#include <QStringList>
#include <QTextStream>
#include <QMetaObject>
#include <QCoreApplication>

class QTailThread : public QThread
{
    Q_OBJECT

public:
    explicit QTailThread (QObject *parant = 0);
    void setFilePath (QString filePath);
    void setHighLight(QList<HightLight> *lst);
    void addHighLight(QList<HightLight> *lst);
    void setHtmlFomarting(bool);
    void quit ();
    void ReloadConfig();
    QList<HightLight> hihgLight ();

protected:
    void run ();

private:
    bool stop;
    bool htmlFormating;
    qint64 positionOnQuit;

    QFile *file;
    QSettings *settings;
    QTextStream *ts;
    QString previousFile;
    QList<HightLight> *lstHighLight;

    void TailFile (QStringList *);
    void SeekAlmostToEnd(qint64);
    void TailSleep(int);
    QString ProcessLine (QString);

signals:
    void NewLogLines (QStringList);

public slots:

};

#endif // QTAILTHREAD_H
