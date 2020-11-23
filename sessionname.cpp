#include "sessionname.h"
#include "ui_sessionname.h"

SessionName::SessionName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionName)
{
    ui->setupUi(this);
}

SessionName::SessionName(QString sessioName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionName)
{
    ui->setupUi(this);
    this->ui->tbSessionName->setText(sessioName);
}

SessionName::~SessionName()
{
    delete ui;
}

void SessionName::accept()
{
    QString name = this->ui->tbSessionName->text();

    if(!name.isEmpty() && !name.isNull())
    {
        this->_sessionName = name;
    }

    this->setResult(QDialog::Accepted);
    this->hide();
}

QString SessionName::GetSessionName()
{
    return this->_sessionName;
}
