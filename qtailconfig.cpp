#include "qtailconfig.h"
#include "ui_qtailconfig.h"

// Constructor ------------------------------------------------------------------------------------
QTailConfig::QTailConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTailConfig)
{
    ui->setupUi(this);
    this->Init();
}

// Destructor -------------------------------------------------------------------------------------
QTailConfig::~QTailConfig()
{
    delete ui;
}

// Slots ------------------------------------------------------------------------------------------
void QTailConfig::MenuSelected(QListWidgetItem *item)
{
    QString itemName = item->data(Qt::UserRole).toString();

    if(this->currentItem != "" && this->currentItem != "Load Default")
    {
        if(CheckToSave() == true)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Config modified.");
            msgBox.setInformativeText("Do you want to save your changes?");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Save);

            int ret = msgBox.exec();

            if(ret == QMessageBox::Save)
            {
                SaveConfig();
            }
        }
    }

    if(itemName == "General" && this->currentItem != itemName)
    {
        ucGeneralConfig *tab = new ucGeneralConfig(this->ui->tbOptions);
        this->SetTab(tab, itemName);
    }

    if(itemName == "File Options" && this->currentItem != itemName)
    {
        ucFileConfig *tab = new ucFileConfig(this->ui->tbOptions);
        this->SetTab(tab, itemName);
    }

    if(itemName == "Edit History" && this->currentItem != itemName)
    {
        ucEditFileHistory *tab = new ucEditFileHistory(this->ui->tbOptions);
        this->SetTab(tab, itemName);
    }

    if(itemName == "Highlight Default" && this->currentItem != itemName)
    {
        ucDefaultHighLight *tab = new ucDefaultHighLight(this->ui->tbOptions);
        this->SetTab(tab, itemName);
    }

    if(itemName == "Load Default")
    {
        this->GenerateDefaultConfig();
    }
}

void QTailConfig::ButtonClicked(QAbstractButton* btClicked)
{
    bool bClose = false;
    QDialogButtonBox::ButtonRole btRole = this->ui->buttonBox->buttonRole(btClicked);

    switch(btRole)
    {
    case QDialogButtonBox::AcceptRole:
        {
            qDebug() << "Accept clicked";

            if(CheckToSave() == true)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Config modified.");
                msgBox.setInformativeText("Do you want to save your changes?");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
                msgBox.setDefaultButton(QMessageBox::Save);

                int ret = msgBox.exec();

                if(ret == QMessageBox::Save)
                {
                    SaveConfig();
                }
            }

            this->setResult(QDialog::Accepted);
            bClose = true;
        }
        break;
    case QDialogButtonBox::ApplyRole:
        {
            qDebug() << "Apply clicked";
            SaveConfig();
        }
        break;
    case QDialogButtonBox::RejectRole:
        {
            qDebug() << "Cancel clicked";
            this->setResult(QDialog::Rejected);
            bClose = true;
        }
        break;
    default:
        {
            qDebug() << "Default" << btRole;
        }
        break;
    }

    if(bClose)
    {
        this->close();
    }
}

// Public Methods -------------------------------------------------------------
bool QTailConfig::ConfigHasChanged()
{
    return this->reloadNeeded;
}

// Private Methods ------------------------------------------------------------
void QTailConfig::Init()
{
    this->setWindowTitle("QTailer - Configuration");
    this->currentItem = "";
    this->reloadNeeded = false;

    QString GenIconPath = QCoreApplication::applicationDirPath ();
    GenIconPath += "/Images/48x48/executable.png";

    QListWidgetItem *itemGen = new QListWidgetItem("General");
    itemGen->setIcon(QIcon(GenIconPath));
    itemGen->setData(Qt::UserRole, "General");


    QString FileIconPath = QCoreApplication::applicationDirPath ();
    FileIconPath += "/Images/48x48/file-black-01.png";

    QListWidgetItem *itemFile = new QListWidgetItem("File");
    itemFile->setIcon(QIcon(FileIconPath));
    itemFile->setData(Qt::UserRole, "File Options");


    QString HistoIconPath = QCoreApplication::applicationDirPath ();
    HistoIconPath += "/Images/128x128/file-history.png";

    QListWidgetItem *fileHistory = new QListWidgetItem("Edit History");
    fileHistory->setIcon(QIcon(HistoIconPath));
    fileHistory->setData(Qt::UserRole, "Edit History");


    QString HighlightDefIconPath = QCoreApplication::applicationDirPath ();
    HighlightDefIconPath += "/Images/48x48/color-text.png";

    QListWidgetItem *highlightDef = new QListWidgetItem("Highlight Default");
    highlightDef->setIcon(QIcon(HighlightDefIconPath));
    highlightDef->setData(Qt::UserRole, "Highlight Default");

    this->ui->lstSelectOption->addItem(itemGen);
    this->ui->lstSelectOption->addItem(itemFile);
    this->ui->lstSelectOption->addItem(fileHistory);
    this->ui->lstSelectOption->addItem(highlightDef);

    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    QSettings sets(configPath, QSettings::IniFormat);
    if(sets.value("system/showconfigdefault", false).toBool() == true)
    {
        QString DefaultIconPath = QCoreApplication::applicationDirPath ();
        DefaultIconPath += "/Images/128x128/text-generic.png";

        QListWidgetItem *itemDefault = new QListWidgetItem("Load Default");
        itemDefault->setIcon(QIcon(DefaultIconPath));
        itemDefault->setData(Qt::UserRole, "Load Default");
        this->ui->lstSelectOption->addItem(itemDefault);
    }

}

void QTailConfig::SetTab(QWidget* tabContaint, QString tabName)
{
    int tabCount = this->ui->tbOptions->count();

    for(int i = 0; i < tabCount; i++)
    {
        this->ui->tbOptions->removeTab(i);
    }

    this->ui->tbOptions->addTab(tabContaint, tabName);
    this->currentItem = tabName;
}

void QTailConfig::GenerateDefaultConfig()
{
    QStringList lst;
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    QSettings sets(configPath, QSettings::IniFormat);

    sets.setValue("general/maxtabnumber", 10);
    sets.setValue("general/linetodisplay",200);
    sets.setValue("general/startcolor", "#9DF0C8");
    sets.setValue("general/stopcolor", "#FFBFAD");
    sets.setValue("general/fileviewer", "/usr/bin/kate");
    sets.setValue("file/history", lst);
    sets.setValue("file/nbfilehistory", 35);
    sets.setValue("file/taildelay", 500);
    sets.setValue("system/showconfigdefault", true);

    sets.sync();
}

bool QTailConfig::CheckToSave()
{
    bool changed = false;

    if(this->currentItem == "General")
    {
        changed = ((ucGeneralConfig*)this->ui->tbOptions->currentWidget())->HasChanged();
    }

    if(this->currentItem == "File Options")
    {
        changed = ((ucFileConfig*)this->ui->tbOptions->currentWidget())->HasChanged();
    }

    if(this->currentItem == "Edit History")
    {
        changed = ((ucEditFileHistory*)this->ui->tbOptions->currentWidget())->HasChanged();
    }

    if(this->currentItem == "Highlight Default")
    {
        changed = ((ucDefaultHighLight*)this->ui->tbOptions->currentWidget())->HasChanged();
    }

    return changed;
}

void QTailConfig::SaveConfig()
{
    this->reloadNeeded = true;

    if(this->currentItem == "General")
    {
        ((ucGeneralConfig*)this->ui->tbOptions->currentWidget())->SaveToConfig();
    }

    if(this->currentItem == "File Options")
    {
        ((ucFileConfig*)this->ui->tbOptions->currentWidget())->SaveToConfig();
    }

    if(this->currentItem == "Edit History")
    {
        ((ucEditFileHistory*)this->ui->tbOptions->currentWidget())->SaveToConfig();
    }

    if(this->currentItem == "Highlight Default")
    {
        ((ucDefaultHighLight*)this->ui->tbOptions->currentWidget())->SaveToConfig();
    }
}
