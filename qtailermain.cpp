#include "qtailermain.h"
#include "ui_qtailermain.h"

#include <QDebug>

QTailerMain::QTailerMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTailerMain)
{   
    ui->setupUi(this);
    this->Init();

    qDebug() << QCoreApplication::applicationDirPath ();

    //this->setWindowTitle(QCoreApplication::applicationDirPath ());
}

QTailerMain::~QTailerMain()
{
    qDebug() << "Destructor QTailerMain";
    delete ui;
    delete this->addTabButton;
}

void QTailerMain::Init()
{
    // Center main window to the screen.
    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    // Load Application Settigns.
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    if(QFileInfo(configPath).exists())
    {
        this->settings = new QSettings(configPath, QSettings::IniFormat);
    }
    else
    {
        // TODO make def config.
        this->settings = new QSettings(configPath, QSettings::IniFormat);
        this->settings->setValue("general/maxtabnumber", 10);
        this->settings->sync();
    }

    // Set application window title.
    QString title = "QTailer [";
    title += VERSION;
    title += "]";

    this->setWindowTitle(title);

    // Init internal fields.
    this->tabCounter = 0;

    QString addPath = QCoreApplication::applicationDirPath ();
    addPath += "/Images/24x24/add.png";

    QString mainIcon = QCoreApplication::applicationDirPath ();
    mainIcon += "/Images/48x48/file_down.png";

    // Create addTab button.
    this->addTabButton = new QPushButton(QIcon(addPath),"Add Tab",ui->tabWidget);
    this->addTabButton->setIconSize(QSize(12,12));
    this->addTabButton->setFlat(true);
    ui->tabWidget->setCornerWidget(this->addTabButton);

    if(this->settings->value("general/enablesession", false).toBool() == true)
    {
        // Load saved session.
        QStringList sesNames = this->settings->value("sessions/names").toStringList();

        // Show the session selection form.
        SessionChooser sessChose (&sesNames, this);
        sessChose.setWindowTitle(title.append(" Session Chooser"));

        if(sessChose.exec() == QDialog::Accepted &&
           sessChose.GetSelectedSession() != DEFAULT_SESSION &&
           sessChose.GetSelectedSession() != "")
        {
            this->isNewSession = false;
            this->currentSessionName = sessChose.GetSelectedSession();

            QString selectedSession = "sessions/";
            selectedSession += sessChose.GetSelectedSession();

            QStringList sessionData = this->settings->value(selectedSession).toStringList();

            QString sess;

            foreach(sess, sessionData)
            {
                CSession s(sess);
                AddTabPage(s);
            }
        }
        else
        {
            this->isNewSession = true;
            this->currentSessionName = "";

            // Add the first tab.
            ucTabContaint *tab = new ucTabContaint(this->tabCounter, ui->tabWidget);
            int index = ui->tabWidget->addTab(tab, QIcon(mainIcon), "No File");
            tab->setTabIndex(index);

            QString tabText(QString::number(index + 1));
            tabText += " - No File";
            ui->tabWidget->setTabText(index, tabText);

            ui->tabWidget->setTabsClosable(false);
            this->tabCounter++;

            QObject::connect(tab, SIGNAL(FileChanged(QString,QString,int)),
                             this, SLOT(selectedFileChanged(QString,QString,int)));
        }
    }
    else
    {
        // Add the first tab.
        ucTabContaint *tab = new ucTabContaint(this->tabCounter, ui->tabWidget);
        int index = ui->tabWidget->addTab(tab, QIcon(mainIcon), "No File");
        tab->setTabIndex(index);

        QString tabText(QString::number(index + 1));
        tabText += " - No File";
        ui->tabWidget->setTabText(index, tabText);

        ui->tabWidget->setTabsClosable(false);
        this->tabCounter++;

        QObject::connect(tab, SIGNAL(FileChanged(QString,QString,int)),
                         this, SLOT(selectedFileChanged(QString,QString,int)));
    }

    // Connect signal to slot.
    QObject::connect(this->addTabButton, SIGNAL(clicked()),
                     this, SLOT(addTabbedButtonClicked()));

    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
                     this, SLOT(closeTabButtonClicked(int)));
}

QWidget* QTailerMain::getTabPage(int index)
{
    return ui->tabWidget->widget(index);
}

void QTailerMain::AddTabPage(CSession s)
{
    QString mainIcon = QCoreApplication::applicationDirPath ();
    mainIcon += "/Images/48x48/file_down.png";

    // Add new tab page.
    ucTabContaint *tab = new ucTabContaint(s, ui->tabWidget);
    this->ui->tabWidget->addTab(tab, QIcon(mainIcon), "No File");

    QString tabText(QString::number(s.GetTabNumber() + 1));
    tabText += " - ";
    tabText += QFileInfo(s.GetFile()).fileName();

    ui->tabWidget->setTabText(s.GetTabNumber(), tabText);

    this->tabCounter++;

    if(this->tabCounter > 1)
    {
        ui->tabWidget->setTabsClosable(true);
    }
    else
    {
        ui->tabWidget->setTabsClosable(false);
    }

    // Connect to tab signals.
    QObject::connect(tab, SIGNAL(FileChanged(QString, QString,int)),
                     this, SLOT(selectedFileChanged(QString,QString,int)));

    if(this->ui->tabWidget->count() >= this->settings->value("general/maxtabnumber", 10).toInt())
    {
        this->addTabButton->setEnabled(false);
    }
    else
    {
        this->addTabButton->setEnabled(true);
    }
}

void QTailerMain::addTabbedButtonClicked()
{
    QString mainIcon = QCoreApplication::applicationDirPath ();
    mainIcon += "/Images/48x48/file_down.png";

    // Add new tab page.
    ucTabContaint *tab = new ucTabContaint(this->tabCounter, ui->tabWidget);
    int index = ui->tabWidget->addTab(tab, QIcon(mainIcon), "No File");
    tab->setTabIndex(index);

    QString tabText(QString::number(index + 1));
    tabText += " - No File";

    ui->tabWidget->setTabText(index, tabText);

    // Set Focus to the new tab page.
    ui->tabWidget->setCurrentIndex(this->tabCounter);
    this->tabCounter++;

    if(this->tabCounter > 1)
    {
        ui->tabWidget->setTabsClosable(true);
    }
    else
    {
        ui->tabWidget->setTabsClosable(false);
    }

    // Connect to tab signals.
    QObject::connect(tab, SIGNAL(FileChanged(QString, QString,int)),
                     this, SLOT(selectedFileChanged(QString,QString,int)));

    if(this->ui->tabWidget->count() >= this->settings->value("general/maxtabnumber", 10).toInt())
    {
        this->addTabButton->setEnabled(false);
    }
    else
    {
        this->addTabButton->setEnabled(true);
    }
}

void QTailerMain::closeTabButtonClicked(int index)
{
    // Remove tab page.
    //ui->tabWidget->removeTab(index);
    this->tabCounter--;

    // Close inner tab widget.
    // This is enought to make the tab to
    // be removed by the tab widget.
    ui->tabWidget->widget(index)->close();
    delete ui->tabWidget->widget(index);

    if(this->ui->tabWidget->count() > 1)
    {
        ui->tabWidget->setTabsClosable(true);
    }
    else
    {
        ui->tabWidget->setTabsClosable(false);
    }

    if(this->ui->tabWidget->count() >= this->settings->value("general/maxtabnumber", 10).toInt())
    {
        this->addTabButton->setEnabled(false);
    }
    else
    {
        this->addTabButton->setEnabled(true);
    }
}

void QTailerMain::selectedFileChanged(QString fileName, QString filePath, int tabIndex)
{
    // Refresh Tab text.
    QString tabText(QString::number(tabIndex + 1));
    tabText += " - ";
    tabText += fileName;
    ui->tabWidget->setTabText(tabIndex, tabText);

//    QString title = "QTailer [";
//    title += VERSION;
//    title += "] - ";
//    title += filePath;

//    this->setWindowTitle(title);
}

void QTailerMain::MenuBarClicked(QAction *action)
{
    if(action->text() == "About")
    {
        About a(this);
        a.exec();
        return;
    }
    else if(action->text() == "Configuration")
    {
        int tabNb = this->ui->tabWidget->count();

        for(int i = 0; i < tabNb; i++)
        {
            ((ucTabContaint*)this->ui->tabWidget->widget(i))->StopTailing();
        }

        QTailConfig c(this);
        c.exec();

        if(c.ConfigHasChanged())
        {
            // Advise children that config has changed.
            for(int i = 0; i < tabNb; i++)
            {
                ((ucTabContaint*)this->ui->tabWidget->widget(i))->ReloadConfig();
            }

            QString configPath = QCoreApplication::applicationDirPath();
            configPath += "/Config/qtailer.ini";

            this->settings = new QSettings(configPath, QSettings::IniFormat);
        }

        for(int i = 0; i < tabNb; i++)
        {

            ((ucTabContaint*)this->ui->tabWidget->widget(i))->StartTailing();
        }

        return;
    }
}

// Override of Widget virtual closeEvent.
void QTailerMain::closeEvent(QCloseEvent *event)
 {
    if(this->settings->value("general/enablesession", false).toBool() == false)
    {
        return;
    }

    SessionName *sessName;

    if(!this->isNewSession)
    {
        sessName = new SessionName(this->currentSessionName, this);
    }
    else
    {
        sessName = new SessionName(this);
    }

    int ret = sessName->exec();

    if(ret == QDialog::Accepted)
    {
        QString sName;
        QStringList sessionData;

        sName = sessName->GetSessionName();

        for(int i = 0; i<this->ui->tabWidget->count(); i++)
        {
            CSession s = ((ucTabContaint*)this->ui->tabWidget->widget(i))->getSessionInfo();
            sessionData.push_back(s.ToString());
        }

        QStringList names = this->settings->value("sessions/names").toStringList();

        if(!names.contains(sName))
        {
            names.push_back(sName);
        }

        QString sessionKey = "sessions/";
        sessionKey += sName;

        this->settings->setValue("sessions/names", names);

        if(!this->isNewSession)
        {
            this->settings->remove(sessionKey);
        }

        this->settings->setValue(sessionKey, sessionData);
        this->settings->sync();
    }
 }

void QTailerMain::TabChange(int tabIndex)
{
//    QString title = "QTailer [";
//    title += VERSION;
//    title += "] ";

//    QString filePath = ((ucTabContaint*)this->ui->tabWidget->widget(tabIndex))->getFilePath();

//    if(filePath != "")
//    {
//        title += "- ";
//        title += filePath;
//    }

//    this->setWindowTitle(title);
}
