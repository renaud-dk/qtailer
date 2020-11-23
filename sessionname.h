#ifndef SESSIONNAME_H
#define SESSIONNAME_H

#include <QDialog>

namespace Ui {
    class SessionName;
}

class SessionName : public QDialog
{
    Q_OBJECT

public:
    explicit SessionName(QWidget *parent = 0);
    explicit SessionName(QString, QWidget *parent = 0);
    ~SessionName();

    QString GetSessionName();

private slots:
    void accept();

private:
    Ui::SessionName *ui;

    QString _sessionName;
};

#endif // SESSIONNAME_H
