#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QString text = "QTailer ";
    text += VERSION;
    this->ui->lbAppName->setText(text);

    QString author = "Build Date ";
    author += __DATE__;
    author += " ";
    author += __TIME__;
    this->ui->lbBuildDate->setText(author);
}

About::~About()
{
    delete ui;
}
