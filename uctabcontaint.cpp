#include "uctabcontaint.h"
#include "ui_uctabcontaint.h"
#include <QMimeData>
#include <QDebug>

// Constructor ----------------------------------------------------------------
/*!
 * \brief ucTabContaint constructor.
 *
 * Construct the user control with default values.
 *
 * @param tabIndex
 *  The number of the tab (will be used for header tab).
 *
 * @param *parent
 *   Pointer of parent Widget.
 */

ucTabContaint::ucTabContaint(int tabIndex, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucTabContaint)
{
    ui->setupUi(this);
    this->tabIndex = tabIndex;
    this->tailThread = NULL;

    Init();
}

/*!
 * \brief ucTabContaint constructor.
 *
 * Construct the user control based on value from a session object.
 *
 * @param session
 *  Object holding the parameter for tab containt (comming from config file).
 *
 * @param *parent
 *   Pointer of parent Widget.
 */

ucTabContaint::ucTabContaint(CSession session,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucTabContaint)
{
    ui->setupUi(this);
    this->tabIndex = session.GetTabNumber();
    this->tailThread = NULL;

    Init(session);
}

// Destructor -----------------------------------------------------------------
/*!
 * \brief ~ucTabContaint destructor.
 *
 * Destroy internal object and release memory.
 */

ucTabContaint::~ucTabContaint()
{
    QString dbgStr = "Destructor ucTabContaint index ";
    dbgStr += QString::number(this->tabIndex);
    qDebug() << dbgStr;
    delete ui;

    delete this->fileInfo;
    delete this->selectFileDialog;

    if(this->tailThread->isRunning() == true)
    {
        this->tailThread->quit();
        this->tailThread->wait(2000);
    }

    delete this->settings;
    delete this->tailThread;
    delete this->lstLine;
    delete this->lstFilteredLine;
}

// Slots ----------------------------------------------------------------------
/*!
 * \brief Slot ActionButtonClicked
 *
 * Callback when user starts tailling. The function bases it behavior on
 * internal flag indicating if tailling is starte or not.
 */

void ucTabContaint::ActionButtonClicked()
{
    if(this->isStarted == false)
    {
        this->setStartView();
    }
    else
    {
        this->setStopView();
    }
}

/*!
 * \brief Slot FileOpenClicked
 *
 * Callback to show open file dialog to select the log file to tail.
 */

void ucTabContaint::FileOpenClicked(void)
{
    // Store preivious file path.
    this->previousFilePath = this->fileInfo->filePath();
    this->selectFileDialog->show();
}

void ucTabContaint::FileSelected(QString fileName)
{
    // Store file info.
    this->fileInfo->setFile(fileName);

    // Store file in combox.
    // Before check if the file is not yet in combo.
    int index = this->ui->cbFileOpen->findText(fileName);

    if(index == -1)
    {
        if(this->ui->cbFileOpen->count() == 0)
        {
            this->ui->cbFileOpen->insertItem(0, this->fileInfo->filePath());
        }
        else
        {
            this->ui->cbFileOpen->addItem(this->fileInfo->filePath());
        }

        this->ui->cbFileOpen->setCurrentIndex(this->ui->cbFileOpen->count() - 1);
    }
    else
    {
        this->ui->cbFileOpen->setCurrentIndex(index);
    }

    SaveToHistory(this->fileInfo->filePath());

    // Advise parent.
    emit FileChanged(this->fileInfo->fileName(), this->fileInfo->filePath(), this->tabIndex);
}

void ucTabContaint::NewLineToDisplay(QStringList lst)
{
    QString str;
    int maxLineToShow = this->settings->value("general/linetodisplay", 200).toInt();

    if(lst.count() > 0)
    {
        // To make sure that the line will be inserted at the end
        // of the text edit.
        QTextCursor c =  this->ui->teFile->textCursor();
        c.movePosition(QTextCursor::End);
        this->ui->teFile->setTextCursor(c);

        //qDebug()<< "Saving" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        foreach(str, lst)
        {
            // Save Local.
            // Revove previous line
            if(this->lstLine->count() == maxLineToShow)
            {
                // Remove old line and add the new one.
                this->lstLine->removeAt(0);
                this->lstLine->push_back(str);
            }
            else
            {
                this->lstLine->push_back(str);
            }

            // Check if filter is enable.
            if(this->ui->ckEnableFilter->isChecked() == true)
            {
                QString filter = this->ui->tbFilter->text();
                QString strOrig = str;

                str = str.remove(QRegExp("(<b><span style=\"color:#......\">)|(</span></b>)"));

                if(str.contains(filter))
                {
                    if(this->lstFilteredLine->count() == maxLineToShow)
                    {
                        // Remove old line and add the new one.
                        this->lstFilteredLine->removeAt(0);
                        this->lstFilteredLine->push_back(strOrig);
                    }
                    else
                    {
                        this->lstFilteredLine->push_back(strOrig);
                    }
                }
            }
        }
        //qDebug()<< "Line count "<<this->lstLine->count()<< " Filtered count "<<this->lstFilteredLine->count();
        //qDebug()<< "Saved" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        //qDebug()<< "Refreshing" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        ReFreshFileShow();
        //qDebug()<< "Refreshed" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

        c =  this->ui->teFile->textCursor();
        c.movePosition(QTextCursor::End);
        this->ui->teFile->setTextCursor(c);
    }
}

void ucTabContaint::ShowSelectHighLight()
{
    SelectHighLight *s = new SelectHighLight(this->tailThread->hihgLight(), this);
    int ret = s->exec();

    this->lstCustHighlight->clear();

    if(ret == QDialog::Accepted)
    {
        // Save selected highlight.
        for(int i = 0; i < s->SelectItems()->count(); i++)
        {
            HightLight elt = s->SelectItems()->at(i);
            if(elt.IsDefault() == false)
            {
                this->lstCustHighlight->push_back(elt);
            }
        }

        // Get the list and pass it to the thread.
        this->tailThread->setHighLight(this->lstDefHighlight);
        this->tailThread->addHighLight(this->lstCustHighlight);
    }

    delete s;
}

void ucTabContaint::OpenInEditor()
{
    QStringList args;
    QString str, prgm = this->settings->value("general/fileviewer", "").toString();

    if(prgm == "")
    {
        QMessageBox box(QMessageBox::Warning, "File Editor", "No file viewer or editor is seted.",
                        QMessageBox::Ok,this,Qt::Dialog);

        box.exec();
        return;
    }

    str = this->fileInfo->filePath();

    if(str.isNull() || str.isEmpty())
    {
        QMessageBox box(QMessageBox::Warning, "Select File", "No file selected.",
                        QMessageBox::Ok,this,Qt::Dialog);

        box.exec();
        return;
    }

    args.push_back(str);
    QProcess::startDetached(prgm, args);
}

void ucTabContaint::SelectFileChanged(QString str)
{
    this->previousFilePath = this->fileInfo->filePath();
    this->fileInfo->setFile(str);

    emit FileChanged(this->fileInfo->fileName(), this->fileInfo->filePath(), this->tabIndex);
}

void ucTabContaint::FilterCheckChange(int state)
{
    if(state == Qt::Checked && this->ui->tbFilter->text() != "")
    {
        this->ui->tbFilter->setEnabled(false);
        FilterText();
    }
    else
    {
        this->lstFilteredLine->clear();
        this->ui->tbFilter->setEnabled(true);
        RestoreFileView();
    }
}

void ucTabContaint::FilterCleared(QString text)
{
    if(text == "" && this->ui->ckEnableFilter->isChecked() == true)
    {
        RestoreFileView();
    }
}

// Public Methods -------------------------------------------------------------
/*!
 * \brief Returns the file that is tailed by the tab.
 */

QString ucTabContaint::getFileName ()
{
    return this->fileInfo->fileName();
}

/*!
 * \brief Returns the absolute path of the file that is tailed.
 */

QString ucTabContaint::getFilePath()
{
    return this->fileInfo->filePath();
}

/*!
 * \brief Returns a session object describing the tab.
 */

CSession ucTabContaint::getSessionInfo()
{
    CSession sess;
    sess.SetTabNumber(this->tabIndex);
    sess.SetFile(this->fileInfo->filePath());
    sess.SetHighLight(*(this->lstCustHighlight));

    return sess;
}

/*!
 * \brief Set the tab number.
 *
 * @param index
 *  Tab number.
 */

void ucTabContaint::setTabIndex (int index)
{
    this->tabIndex = index;
}

void ucTabContaint::StopTailing()
{
    this->prevStartState = this->isStarted;

    if(this->isStarted)
    {
        setStopView();
    }
}

void ucTabContaint::StartTailing()
{
    if(this->prevStartState && !this->isStarted)
    {
        setStartView();
    }
}

void ucTabContaint::ReloadConfig()
{
    delete this->settings;

    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    // Save current combobox index.
    int index = this->ui->cbFileOpen->currentIndex();
    this->ui->cbFileOpen->clear();

    // Fill in combo file open with history.
    QStringList hLst = this->settings->value("file/history").toStringList();
    QString str;

    foreach(str, hLst)
    {
        this->ui->cbFileOpen->addItem(str);
    }

    this->ui->cbFileOpen->setCurrentIndex(index);

    // Inner thread must also relaod the config file.
    if(this->tailThread != NULL)
    {
        this->tailThread->ReloadConfig();

        // Reload default highlight.
        QStringList lstHighlightStr = this->settings->value("hightlight/default").toStringList();
        this->lstDefHighlight->clear();

        if(lstHighlightStr.count() > 0)
        {
            QString str;

            foreach(str, lstHighlightStr)
            {
                this->lstDefHighlight->push_back(HightLight(str, true));
            }
        }

        this->tailThread->setHighLight(this->lstDefHighlight);
        this->tailThread->addHighLight(this->lstCustHighlight);
    }
}

// Protected Methods ----------------------------------------------------------
void ucTabContaint::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        if(!this->isStarted)
        {
            event->acceptProposedAction();
        }
    }
}

void ucTabContaint::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
    {
        return;
    }

    // Store dropbed file.
    this->fileInfo->setFile(urls.first().toLocalFile());  

    // Store file in combox.
    // Before check if the file is not yet in combo.
    int index = -1;

    for(int i = 0; i < this->ui->cbFileOpen->count(); i++)
    {
        if(this->ui->cbFileOpen->itemText(i) == this->fileInfo->filePath())
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        this->ui->cbFileOpen->addItem(this->fileInfo->filePath());
        int itemIndex = ui->cbFileOpen->count();
        this->ui->cbFileOpen->setCurrentIndex(itemIndex -1);
    }
    else
    {
        this->ui->cbFileOpen->setCurrentIndex(index);
    }

    SaveToHistory(this->fileInfo->filePath());

    // Advise parent.
    emit FileChanged(this->fileInfo->fileName(), this->fileInfo->filePath(), this->tabIndex);
}


// Private Methods ------------------------------------------------------------
void ucTabContaint::Init()
{
    // Initialize internal fields.
    this->firstRun = true;
    this->previousFilePath = "";
    this->fileInfo = new QFileInfo();
    this->tailThread = new QTailThread(this);
    this->lstLine = new QStringList();
    this->lstFilteredLine = new QStringList();

    this->lstDefHighlight = new QList<HightLight>();
    this->lstCustHighlight = new QList<HightLight>();

    this->selectFileDialog = new QFileDialog(this, Qt::Dialog);
    this->selectFileDialog->setFileMode(QFileDialog::ExistingFile);

    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    this->tailThread->ReloadConfig();

    // Load default highlight.
    QStringList lstHighlightStr = this->settings->value("hightlight/default").toStringList();

    if(lstHighlightStr.count() > 0)
    {
        QString str;

        foreach(str, lstHighlightStr)
        {
            this->lstDefHighlight->push_back(HightLight(str, true));
        }
    }

    if(this->lstDefHighlight->count() > 0)
    {
        this->tailThread->setHighLight(this->lstDefHighlight);
    }

    // Fill in combo file open with history.
    QStringList hLst = this->settings->value("file/history").toStringList();
    QString str;

    if(hLst.count() > 0)
    {
        foreach(str, hLst)
        {
            this->ui->cbFileOpen->addItem(str);
        }
    }

    this->ui->teFile->setAcceptDrops(false);
    this->ui->tbFilter->setAcceptDrops(false);
    this->ui->gbFile->setAcceptDrops(false);
    this->ui->gbSettings->setAcceptDrops(false);
    this->setAcceptDrops(true);

    this->setStopView();

    QObject::connect(this->selectFileDialog, SIGNAL(fileSelected(QString)),
                     this, SLOT(FileSelected(QString)));

    QObject::connect(this->tailThread, SIGNAL(NewLogLines(QStringList)),
                     this, SLOT(NewLineToDisplay(QStringList)));
}

void ucTabContaint::Init(CSession session)
{
    Init();

    // Retrive tailing file.
    for(int i = 0; i < this->ui->cbFileOpen->count(); i++)
    {
        qDebug() << i << " item " << (QString)this->ui->cbFileOpen->itemText(i) << endl;

        if(this->ui->cbFileOpen->itemText(i) == session.GetFile())
        {
            this->ui->cbFileOpen->setCurrentIndex(i);
        }
    }

    // Retrieve Highlight value.
    HightLight elt;

    foreach(elt, session.GetHighLight())
    {
        this->lstCustHighlight->push_back(elt);
    }

    this->tailThread->addHighLight(this->lstCustHighlight);
}

void ucTabContaint::setStopView()
{
    // Change Text component color.
    int tailingDelay = this->settings->value("file/taildelay", 500).toInt();
    QColor c(this->settings->value("general/stopcolor", "#FFBFAD").toString());
    c.setAlpha(200);
    QPalette p = ui->teFile->palette();

    p.setColor(QPalette::Base, c);
    ui->teFile->setPalette(p);

    QString playPath = QCoreApplication::applicationDirPath ();
    playPath += "/Images/48x48/play.png";

    // Change button and other component behavior.
    ui->btStart->setText("Start !");
    ui->btStart->setIcon(QIcon(playPath));
    ui->cbFileOpen->setEnabled(true);
    ui->btFileOpen->setEnabled(true);

    this->isStarted = false;

    // First run protection.
    if(!this->firstRun)
    {
        this->tailThread->quit();

        this->tailThread->wait(3 * tailingDelay);

        while(!this->tailThread->isFinished())
        {
            qDebug() << "Thread not yet finished";
        }

        qDebug() << "Thread Finished";
    }
}

void ucTabContaint::setStartView()
{
    // Check file selection.
    QString str = this->fileInfo->filePath();

    if(str.isNull() || str.isEmpty())
    {
        str = this->ui->cbFileOpen->currentText();

        this->fileInfo->setFile(str);

        if(!this->fileInfo->exists())
        {
            this->fileInfo->setFile("");

            QMessageBox box(QMessageBox::Warning, "Select File", "Please select a file",
                    QMessageBox::Ok,this,Qt::Dialog);

            box.exec();
            return;
        }
    }

    if(this->previousFilePath == "")
    {
        this->previousFilePath = this->fileInfo->filePath();
    }

    if(this->previousFilePath != str)
    {
        this->ui->teFile->clear();
        //this->lines.clear();
        this->lstLine->clear();
    }

    emit FileChanged(this->fileInfo->fileName(), this->fileInfo->filePath(), this->tabIndex);

    // Change Text component color.
    QColor c(this->settings->value("general/startcolor", "#9DF0C8").toString());
    c.setAlpha(200);
    QPalette p = ui->teFile->palette();

    p.setColor(QPalette::Base, c);
    ui->teFile->setPalette(p);

    QString stopPath = QCoreApplication::applicationDirPath ();
    stopPath += "/Images/48x48/stop.png";

    // Change button and other component behavior.
    ui->btStart->setText("Stop !");
    //ui->btStart->setIcon(QIcon("./Images/48x48/stop.png"));
    ui->btStart->setIcon(QIcon(stopPath));
    ui->cbFileOpen->setEnabled(false);
    ui->btFileOpen->setEnabled(false);

    this->isStarted = true;

    // First run protection.
    if(this->firstRun)
    {
        this->firstRun = false;
    }

    this->tailThread->setFilePath(this->fileInfo->filePath());
    this->tailThread->setHtmlFomarting(true);

    // Start tailer thread.
    this->tailThread->start();
}

void ucTabContaint::ReFreshFileShow()
{
    QString str, s;

    this->ui->teFile->clear();

    if(this->ui->ckEnableFilter->isChecked() == true)
    {
        foreach(s, *(this->lstFilteredLine))
        {
            str+= s;
        }
    }
    else
    {
        foreach(s, *(this->lstLine))
        {
            str+= s;
        }
    }

    this->ui->teFile->insertHtml(str);
}

void ucTabContaint::FilterText()
{
    QString str;
    QString filter = this->ui->tbFilter->text();
    int maxLineToShow = this->settings->value("general/linetodisplay", 200).toInt();

    if(filter == "")
    {
        return;
    }

    this->ui->teFile->clear();

    foreach(str, *(this->lstLine))
    {
        QString strOrig = str;
        str = str.remove(QRegExp("(<b><span style=\"color:#......\">)|(</span></b>)"));

        if(str.contains(filter))
        {
            if(this->lstFilteredLine->count() == maxLineToShow)
            {
                // Remove old line and add the new one.
                this->lstFilteredLine->removeAt(0);
                this->lstFilteredLine->push_back(strOrig);
            }
            else
            {
                this->lstFilteredLine->push_back(strOrig);
            }
        }
    }

    ReFreshFileShow();
}

void ucTabContaint::SaveToHistory (QString filePath)
{
    QStringList hLst = this->settings->value("file/history").toStringList();
    int maxNbHistory = this->settings->value("file/nbfilehistory", 35).toInt();

    if(!hLst.contains(filePath))
    {
        if(hLst.count() >= maxNbHistory)
        {
            hLst.removeAt(0);
        }

        hLst.push_back(filePath);
    }

    this->settings->setValue("file/history", hLst);
    this->settings->sync();
}

void ucTabContaint::RestoreFileView()
{
    QString restore = "<p>";
    QString s;

    this->ui->teFile->clear();

    foreach(s, *(this->lstLine))
    {
        s.remove("<p>");
        s.remove("</p>");
        restore += s;
    }

    restore += "</p>";

    this->ui->teFile->insertHtml(restore);

    QTextCursor c =  this->ui->teFile->textCursor();
    c.movePosition(QTextCursor::End);
    this->ui->teFile->setTextCursor(c);
}
