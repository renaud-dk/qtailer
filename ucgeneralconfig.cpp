#include "ucgeneralconfig.h"
#include "ui_ucgeneralconfig.h"


// Constructor - Destructor ---------------------------------------------------
ucGeneralConfig::ucGeneralConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucGeneralConfig)
{
    ui->setupUi(this);
    this->Init();
}

ucGeneralConfig::~ucGeneralConfig()
{
    delete ui;
    delete this->selectFileDialog;
    delete this->settings;
}

// Public Methods -------------------------------------------------------------
bool ucGeneralConfig::HasChanged()
{
    bool bRet = false;

    if(this->curMaxTabNumber != this->settings->value("general/maxtabnumber",10).toInt() && bRet == false)
    {
        bRet = true;
    }
    else if(this->curLineToDisplay != this->settings->value("general/linetodisplay",200).toInt() && bRet == false)
    {
        bRet = true;
    }
    else if(this->curStartColor != this->settings->value("general/startcolor", "#9DF0C8").toString() && bRet == false)
    {
        bRet = true;
    }
    else if(this->curStopColor != this->settings->value("general/stopcolor", "#FFBFAD").toString() && bRet == false)
    {
        bRet = true;
    }
    else if(this->settings->value("general/fileviewer").toString() != this->curTextEditor && bRet == false)
    {
        bRet = true;
    }
    else if(this->settings->value("general/enablesession").toBool() != this->enableSession && bRet == false)
    {
        bRet = true;
    }
    return bRet;
}

void ucGeneralConfig::SaveToConfig()
{
    // Get config value from internal member and set value to config.
    this->settings->setValue("general/maxtabnumber",this->curMaxTabNumber);
    this->settings->setValue("general/linetodisplay",this->curLineToDisplay);
    this->settings->setValue("general/startcolor",this->curStartColor);
    this->settings->setValue("general/stopcolor",this->curStopColor);
    this->settings->setValue("general/fileviewer", this->curTextEditor);
    this->settings->setValue("general/enablesession", this->enableSession);

    // Write config.
    this->settings->sync();
}

// Slots ----------------------------------------------------------------------
void ucGeneralConfig::SetStartColor()
{
    QColor color;
    color = QColorDialog::getColor(this->settings->value("general/startcolor", "#9DF0C8").toString(), this);

    //this->settings->setValue("general/startcolor", color.toRgb());

    QString sColor = "background-color: ";
    sColor += color.name();

    this->ui->btSelectStartColor->setStyleSheet(sColor);
    this->curStartColor = color.name();
}

void ucGeneralConfig::SetStopColor()
{
    QColor color;
    color = QColorDialog::getColor(this->settings->value("general/stopcolor", "#FFBFAD").toString(), this);

    QString sColor = "background-color: ";
    sColor += color.name();

    this->ui->btSelectStopColor->setStyleSheet(sColor);
    this->curStopColor = color.name();
}

void ucGeneralConfig::FileSelectClicked()
{
    this->selectFileDialog->show();
}

void ucGeneralConfig::FileSelected(QString fileName)
{
    this->curTextEditor = fileName;

    QFileInfo info(this->curTextEditor);
    this->ui->lbEditorName->setText(info.fileName());
}

void ucGeneralConfig::MaxTabValueChanged(int value)
{
    this->curMaxTabNumber = value;
}

void ucGeneralConfig::LineToDisplayValueChanged(int value)
{
    this->curLineToDisplay = value;
}

void ucGeneralConfig::EnableSessionChanged(int state)
{
    if(state == Qt::Checked)
    {
        this->enableSession = true;
        this->ui->ckEnableSession->setText("Enabled");
    }
    else
    {
        this->enableSession = false;
        this->ui->ckEnableSession->setText("Disabled");
    }
}

// Private Methods ------------------------------------------------------------
void ucGeneralConfig::Init()
{
    // Load config file.
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    // Initialize internal fields
    this->selectFileDialog = new QFileDialog(this, Qt::Dialog);
    this->selectFileDialog->setFileMode(QFileDialog::ExistingFile);

    // Fill in spin box.
    this->curMaxTabNumber = this->settings->value("general/maxtabnumber",10).toInt();
    this->ui->sbMaxNbTab->setValue(this->curMaxTabNumber);

    this->curLineToDisplay = this->settings->value("general/linetodisplay",200).toInt();
    this->ui->sbLineToDisplay->setValue(this->curLineToDisplay);

    // Get text editor command.
    this->curTextEditor = this->settings->value("general/fileviewer").toString();

    QFileInfo info(this->curTextEditor);
    this->ui->lbEditorName->setText(info.fileName());

    // Change color button selection button.
    this->ui->btSelectStartColor->setAutoFillBackground(true);
    this->ui->btSelectStopColor->setAutoFillBackground(true);

    // Apply color from config file.
    this->curStartColor = this->settings->value("general/startcolor", "#9DF0C8").toString();
    QString color = "background-color: ";
    color += this->curStartColor;

    this->ui->btSelectStartColor->setStyleSheet(color);

    // Apply color from config file.
    this->curStopColor = this->settings->value("general/stopcolor", "#FFBFAD").toString();
    color = "background-color: ";
    color += this->curStopColor;

    this->ui->btSelectStopColor->setStyleSheet(color);

    // Session enabled.
    this->enableSession = this->settings->value("general/enablesession", false).toBool();

    if(this->enableSession == true)
    {
        this->ui->ckEnableSession->setCheckState(Qt::Checked);
        this->ui->ckEnableSession->setText("Enabled");
    }
    else
    {
        this->ui->ckEnableSession->setCheckState(Qt::Unchecked);
        this->ui->ckEnableSession->setText("Disabled");
    }

    // Connect signal and slots.
    QObject::connect(this->selectFileDialog, SIGNAL(fileSelected(QString)),
                     this, SLOT(FileSelected(QString)));
}
