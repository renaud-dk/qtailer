#include "sessionchooser.h"
#include "ui_sessionchooser.h"

// Constructor ----------------------------------------------------------------
SessionChooser::SessionChooser(QStringList *sessionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionChooser)
{
    ui->setupUi(this);

    this->sess = sessionName;
    this->Init();
}


// Destructor -----------------------------------------------------------------
SessionChooser::~SessionChooser()
{
    delete ui;
}


// Slots ----------------------------------------------------------------------
void SessionChooser::SelectedSessionChanged()
{
    this->selectedSession = this->ui->lwSession->currentItem()->text();
}

void SessionChooser::SessionSelected(QListWidgetItem *item)
{
    this->selectedSession = item->text();
    this->accept();
}


// Public Methods -------------------------------------------------------------
QString SessionChooser::GetSelectedSession()
{
    return this->selectedSession;
}


// Private Methods ------------------------------------------------------------
void SessionChooser::Init()
{
    // Center dialog form.
    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    // Add default session cleaned layout.
    this->ui->lwSession->addItem(DEFAULT_SESSION);

    // Fill in all other session name.
    QString str;

    foreach(str, *(this->sess))
    {
        this->ui->lwSession->addItem(str);
    }
}
