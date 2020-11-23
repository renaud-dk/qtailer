#include "qtailthread.h"
#include <QDebug>

// Constructor ------------------------------------------------------------------------------------
QTailThread::QTailThread(QObject *parent) :
        QThread(parent)
{
    this->lstHighLight = new QList<HightLight>();
    this->htmlFormating = true;

    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);
    this->file = 0;
    this->previousFile = "";
}

// Public Methods ---------------------------------------------------------------------------------
void QTailThread::quit()
{
    qDebug()<< "Quit call.";
    this->stop = true;

    this->previousFile = this->file->fileName();
}

void QTailThread::ReloadConfig()
{
    delete this->settings;

    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);
}

// Properties -------------------------------------------------------------------------------------
void QTailThread::setFilePath(QString filePath)
{
    if(this->file == NULL)
    {
        this->file = new QFile(filePath);
    }

    this->file->setFileName(filePath);
}

void QTailThread::setHighLight(QList<HightLight> *lst)
{
    this->lstHighLight->clear();

    // Add the new highlight word.
    for(int i = 0; i<lst->count(); i++)
    {
        HightLight elt = lst->at(i);
        this->lstHighLight->push_back(elt);
    }
}

void QTailThread::addHighLight(QList<HightLight> *lst)
{
    // Add the new highlight word.
    for(int i = 0; i<lst->count(); i++)
    {
        HightLight elt = lst->at(i);
        this->lstHighLight->push_back(elt);
    }
}

void QTailThread::setHtmlFomarting(bool enable)
{
    this->htmlFormating = enable;
}

QList<HightLight> QTailThread::hihgLight ()
{
    return *(this->lstHighLight);
}

// Portected Methods ------------------------------------------------------------------------------
void QTailThread::run()
{
    int tailDelay = this->settings->value("file/taildelay", 500).toInt();
    int maxNbLine = this->settings->value("general/linetodisplay", 200).toInt();

    QStringList lst;
    this->stop = false;

    if(!this->file->isOpen())
    {
        //qDebug()<< "Opening file " << QDateTime::currentDateTime().toString("hh:mm:ss");
        this->file->open(QIODevice::ReadOnly);
        //qDebug()<< "Opened " << QDateTime::currentDateTime().toString("hh:mm:ss");
    }

    this->ts = new QTextStream(this->file);

    // Process starting the max number of line to show.
    // assuming that 1 line will not exceed 250 character.
    SeekAlmostToEnd(maxNbLine * 250);

    while(!this->stop)
    {
        if(!this->file->exists(this->file->fileName()))
        {
            if(this->htmlFormating)
            {
                lst.push_back("File not found !!!<br />");
            }
            else
            {
                lst.push_back("File not found !!!\r\n");
            }
        }
        else
        {
            if(!this->file->isOpen())
            {
                //qDebug()<< "Opening file " << QDateTime::currentDateTime().toString("hh:mm:ss");
                this->file->open(QIODevice::ReadOnly);
                //qDebug()<< "Opened " << QDateTime::currentDateTime().toString("hh:mm:ss");
            }

            this->TailFile(&lst);
        }

        // Is there something new to display?
        if(lst.count() > 0)
        {
            // Advise form we something new to display.
            emit NewLogLines(lst);
        }
        
        // Sleep a while.
        TailSleep(tailDelay);
        
        // Clear list for next loop.
        lst.clear();
    }

    // Save current position on quit to restore in case same file is started again.
    this->positionOnQuit = this->ts->pos();

    qDebug() << "Position on quit : " << this->positionOnQuit;

    if(this->file->isOpen())
    {
       this->file->close();
    }

    delete this->ts;

    //qDebug() << "Call exec.";
    //this->exec();

    //qDebug() << "Thread will exit run loop.";
}

// Privates Methods -------------------------------------------------------------------------------
void QTailThread::TailSleep(int mSleepTime)
{
    int nbLoop = mSleepTime / 25;
    int rest = mSleepTime % 25;

    for(int i = 0; i<nbLoop; i++)
    {
        if(this->stop)
        {
            return;
        }

        this->msleep(25);
    }

    if(this->stop)
    {
        return;
    }

    this->msleep(rest);
}

void QTailThread::TailFile(QStringList *lst)
{
    QString line = ts->readLine();

    while(!line.isNull())
    {
        QString strFormated = ProcessLine(line);
        lst->push_back(strFormated);
        line = ts->readLine();
    }
}

QString QTailThread::ProcessLine(QString aLine)
{
    QString line = aLine;
    HightLight elt;
    QString search;
    QString color;

    if(this->htmlFormating)
    {
        line = line.replace('<', "&lt;");
        line = line.replace('>', "&gt;");

        //line.insert(0,"<p>");

        for(int i = 0; i < this->lstHighLight->count(); i++)
        {
            elt = this->lstHighLight->at(i);
            search = elt.GetHighLightString();
            color = elt.GetHighLigthColor();

            if(line.contains(search))
            {
                int begin = line.indexOf(search);
                QString s = "<b><span style=\"color:";
                s+=color;
                s+="\">";
                line.insert(begin,s);
                line.insert(s.length() + search.length() + begin, "</span></b>");
            }
        }

        //line += "<br /></p>";
        line += "<br />";
    }

    return line;
}

void QTailThread::SeekAlmostToEnd(qint64 sizeToKeep)
{
    if(this->previousFile != this->file->fileName())
    {
        // Get file total size.
        qint64 totalSize = this->file->size();

        if(totalSize > sizeToKeep)
        {
            // Process only last sizeToKeep of file.
            // --> seek almost at the end of the file.
            ts->seek(totalSize - (sizeToKeep));

            // Discard next line because we are
            // more likekly in the middle of a line.
            ts->readLine();
        }
    }
    else // File did not change restore position on quit.
    {
        qDebug() << "Seek to previous location : " << this->positionOnQuit;
        this->ts->seek(this->positionOnQuit);
    }
}


