#include "selecthighlight.h"
#include "ui_selecthighlight.h"

SelectHighLight::SelectHighLight(QList<HightLight> lst, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectHighLight)
{
    this->ui->setupUi(this);

    this->Init(lst);
}

SelectHighLight::~SelectHighLight()
{
    delete ui;
}

QList<HightLight> *SelectHighLight::SelectItems()
{
    return this->lstElt;
}

// Slots ----------------------------------------------------------------------
void SelectHighLight::ShowColorChooser(void)
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

void SelectHighLight::AddClicked(void)
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
    this->lstElt->push_back(HightLight(this->currentWord, this->currentColor));

    this->ui->tbWord->setText("");
    this->currentWord = "";
}

void SelectHighLight::RemoveClicked()
{
    int selectedRow = this->ui->tableWidget->currentRow();

    if(selectedRow != -1)
    {
        this->ui->tableWidget->removeRow(selectedRow);
        this->lstElt->removeAt(selectedRow);
    }
}

// Privates Methods -----------------------------------------------------------
void SelectHighLight::Init(QList<HightLight> lst)
{
    // Initialize internal field.
    this->currentColor = "";
    this->currentWord = "";
    this->lstElt = new QList<HightLight>(lst);

    // Init table widget.
    this->ui->tableWidget->setColumnCount(3);

    QHeaderView *h = this->ui->tableWidget->horizontalHeader();
    h->setSectionResizeMode(QHeaderView::Stretch);

    QStringList horzHeaders;
    horzHeaders << "Word" << "Color" << "Overview";

    this->ui->tableWidget->setHorizontalHeaderLabels(horzHeaders);
    this->ui->tableWidget->setHorizontalHeader(h);

    if(lst.count() > 0)
    {
        HightLight elt;

        for(int i = 0; i<this->lstElt->count(); i++ )
        {
            elt = this->lstElt->at(i);

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
        }
    }
}
