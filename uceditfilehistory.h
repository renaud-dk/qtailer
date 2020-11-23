#ifndef UCEDITFILEHISTORY_H
#define UCEDITFILEHISTORY_H

#include <QWidget>
#include <QSettings>
#include <QStringList>

namespace Ui {
    class ucEditFileHistory;
}

class ucEditFileHistory : public QWidget
{
    Q_OBJECT

public:
    explicit ucEditFileHistory(QWidget *parent = 0);
    ~ucEditFileHistory();

    bool HasChanged();
    void SaveToConfig();

private slots:
    void RemoveClicked();

private:
    Ui::ucEditFileHistory *ui;
    QSettings *settings;

    QStringList history;

    void Init();
};

#endif // UCEDITFILEHISTORY_H
