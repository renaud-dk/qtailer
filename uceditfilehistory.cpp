#include "uceditfilehistory.h"
#include "ui_uceditfilehistory.h"

// Constructor ------------------------------------------------------------------------------------
ucEditFileHistory::ucEditFileHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucEditFileHistory)
{
    ui->setupUi(this);
    Init();
}

// Destructor -------------------------------------------------------------------------------------
ucEditFileHistory::~ucEditFileHistory()
{
    delete ui;
    delete this->settings;
}

// Public Methods ---------------------------------------------------------------------------------
bool ucEditFileHistory::HasChanged()
{
    bool bRet = false;

    if(this->history.count() != this->settings->value("file/history").toStringList().count())
    {
        bRet = true;
    }

    return bRet;
}

void ucEditFileHistory::SaveToConfig()
{
    QString str;
    this->settings->setValue("file/history", this->history);

    // Write config.
    this->settings->sync();

    this->ui->lwFileHitsory->clear();
    foreach(str, this->history)
    {
        this->ui->lwFileHitsory->addItem(str);
    }
}

// Slots ------------------------------------------------------------------------------------------
void ucEditFileHistory::RemoveClicked()
{
    int index = this->ui->lwFileHitsory->currentRow();

    if(index >= 0)
    {
        this->history.removeAt(index);

        this->ui->lwFileHitsory->currentItem()->setBackgroundColor(QColor("#FF5E5B"));
    }
}

// Privates Methods -------------------------------------------------------------------------------
void ucEditFileHistory::Init()
{
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    this->history = this->settings->value("file/history").toStringList();
    QString str;

    foreach(str, this->history)
    {
        this->ui->lwFileHitsory->addItem(str);
    }
}
