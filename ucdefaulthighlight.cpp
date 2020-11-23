#include "ucdefaulthighlight.h"
#include "ui_ucdefaulthighlight.h"

ucDefaultHighLight::ucDefaultHighLight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ucDefaultHighLight)
{
    ui->setupUi(this);
    this->Init();
}

ucDefaultHighLight::~ucDefaultHighLight()
{
    delete ui;
}

// Public Methods ---------------------------------------------------------------------------------
bool ucDefaultHighLight::HasChanged()
{
    return this->hasChanged;
}

void ucDefaultHighLight::SaveToConfig()
{
    QStringList lst;
    HightLight elt;

    for(int i = 0; i<this->lstDefHighLight->count(); i++)
    {
        elt = this->lstDefHighLight->at(i);
        lst.push_back(elt.ToString());
    }

    this->settings->setValue("hightlight/default", lst);

    // Write config.
    this->settings->sync();

    this->hasChanged = false;
}

// Slots ------------------------------------------------------------------------------------------
void ucDefaultHighLight::ShowColorChooser()
{
    QColor color;

    if(this->currentColor.isEmpty() || this->currentColor.isNull())
    {
        color = QColorDialog::getColor(Qt::white, this);
    }
    else
    {
        color = QColorDialog::getColor(QColor(this->currentColor), this);
    }

    this->currentColor = color.name();
}

void ucDefaultHighLight::AddClicked()
{
    this->currentWord = this->ui->tbWord->text();

    if(this->currentWord.isEmpty())
    {
        QMessageBox box(QMessageBox::Warning, "Select Word", "Please appen a word to highlight",
                        QMessageBox::Ok,this,Qt::Dialog);

        box.exec();
        return;
    }

    if(this->currentColor.isEmpty())
    {

        QMessageBox box(QMessageBox::Warning, "Select Color", "Please select an highlight color",
                        QMessageBox::Ok,this,Qt::Dialog);

        box.exec();
        return;
    }

    QTableWidgetItem *newWord = new QTableWidgetItem(this->currentWord, 0);
    QTableWidgetItem *newColor = new QTableWidgetItem(this->currentColor, 0);
    QTableWidgetItem *newOverview = new QTableWidgetItem(this->currentWord, 0);

    int row = this->ui->tableWidget->rowCount();

    this->ui->tableWidget->insertRow(row);
    this->ui->tableWidget->setItem(row, 0, newWord);
    this->ui->tableWidget->setItem(row, 1, newColor);
    this->ui->tableWidget->setItem(row, 2, newOverview);

    this->ui->tableWidget->item(row,1)->setBackground(QBrush(QColor(this->currentColor)));
    this->ui->tableWidget->item(row,2)->setForeground(QBrush(QColor(this->currentColor)));

    // add in list.
    this->lstDefHighLight->push_back(HightLight(this->currentWord, this->currentColor, true));
    this->hasChanged = true;

    this->ui->tbWord->setText("");
    this->currentWord = "";
}

void ucDefaultHighLight::RemoveClicked()
{
    int selectedRow = this->ui->tableWidget->currentRow();

    if(selectedRow != -1)
    {
        this->ui->tableWidget->removeRow(selectedRow);
        this->lstDefHighLight->removeAt(selectedRow);

        this->hasChanged = true;
    }
}

// Private Methods --------------------------------------------------------------------------------
void ucDefaultHighLight::Init()
{
    this->hasChanged = false;

    // Load config file.
    QString configPath = QCoreApplication::applicationDirPath();
    configPath += "/Config/qtailer.ini";

    this->settings = new QSettings(configPath, QSettings::IniFormat);

    // Init Highlight internal list.
    this->lstDefHighLight = new QList<HightLight>();

    // Init table widget.
    this->ui->tableWidget->setColumnCount(3);

    QHeaderView *h = this->ui->tableWidget->horizontalHeader();
    h->setSectionResizeMode(QHeaderView::Stretch);

    QStringList horzHeaders;
    horzHeaders << "Word" << "Color" << "Overview";

    this->ui->tableWidget->setHorizontalHeaderLabels(horzHeaders);
    this->ui->tableWidget->setHorizontalHeader(h);

    QStringList lstEltStr = this->settings->value("hightlight/default").toStringList();

    if(lstEltStr.count() > 0)
    {
        QString str;

        foreach(str, lstEltStr)
        {
            HightLight elt(str, true);

            QTableWidgetItem *word = new QTableWidgetItem(elt.GetHighLightString(), 0);
            QTableWidgetItem *color = new QTableWidgetItem(elt.GetHighLigthColor(), 0);
            QTableWidgetItem *overview = new QTableWidgetItem(elt.GetHighLightString(), 0);

            int row = this->ui->tableWidget->rowCount();

            this->ui->tableWidget->insertRow(row);
            this->ui->tableWidget->setItem(row, 0, word);
            this->ui->tableWidget->setItem(row, 1, color);
            this->ui->tableWidget->setItem(row, 2, overview);

            this->ui->tableWidget->item(row,1)->setBackground(QBrush(QColor(elt.GetHighLigthColor())));
            this->ui->tableWidget->item(row,2)->setForeground(QBrush(QColor(elt.GetHighLigthColor())));

            this->lstDefHighLight->push_back(elt);
        }
    }
}
