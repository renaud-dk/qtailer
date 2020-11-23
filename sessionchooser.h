#ifndef SESSIONCHOOSER_H
#define SESSIONCHOOSER_H

#include <QDebug>
#include <QDialog>
#include <QStringList>
#include <QApplication>
#include <QDesktopWidget>
#include <QListWidgetItem>
#include "csession.h"
#include "constant.h"

namespace Ui {
    class SessionChooser;
}

class SessionChooser : public QDialog
{
    Q_OBJECT

public:
    explicit SessionChooser(QStringList *sessionName, QWidget *parent = 0);
    ~SessionChooser();

    QString GetSelectedSession();

private slots:
    void SelectedSessionChanged();
    void SessionSelected(QListWidgetItem *);

private:
    Ui::SessionChooser *ui;
    QStringList *sess;
    QString selectedSession;

    void Init();
};

#endif // SESSIONCHOOSER_H
