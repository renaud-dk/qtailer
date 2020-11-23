#include "ucfileconfig.h"
#include "ui_ucfileconfig.h"

// Constructor - Destructor ---------------------------------------------------
ucFileConfig::ucFileConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucFileConfig)
{
    ui->setupUi(this);

    this->Init();
}

ucFileConfig::~ucFileConfig()
{
    delete ui;

    delete this->settings;
}

// Public Methods -------------------------------------------------------------
bool ucFileConfig::HasChanged()
{
    bool bRet = false;

    if(this->curNbFileHistory != this->settings->value("file/nbfilehistory",35).toInt() && bRet == false)
    {
        bRet = true;
    }
    else if(this->curTailDelay != this->settings->value("file/taildelay",500).toInt() && bRet == false)
    {
        bRet = true;
    }

    return bRet;
}

void ucFileConfig::SaveToConfig()
{
    // Get config value from internal member and set value to config.
    this->settings->setValue("file/nbfilehistory",this->curNbFileHistory);
    this->settings->setValue("file/taildelay",this->curTailDelay);

    // Write config.
    this->settings->sync();
}

// Slots ----------------------------------------------------------------------
void ucFileConfig::NbFileHistoryValueChanged(int value)
{
    this->curNbFileHistory = value;
}

void ucFileConfig::TailDelayValueChanged(int value)
{
    this->curTailDelay = value;
}

// Private Methods ------------------------------------------------------------
void ucFileConfig::Init()
{
    // Load config file.
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    // Fill in spin box.
    this->ui->sbNbFileHistory->setValue(this->settings->value("file/nbfilehistory",35).toInt());
    this->ui->sbTailDelay->setValue(this->settings->value("file/taildelay", 500).toInt());
}
