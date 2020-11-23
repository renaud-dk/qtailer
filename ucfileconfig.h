#ifndef UCFILECONFIG_H
#define UCFILECONFIG_H

#include <QWidget>
#include <QSettings>

namespace Ui {
    class ucFileConfig;
}

class ucFileConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ucFileConfig(QWidget *parent = 0);
    ~ucFileConfig();

    bool HasChanged();
    void SaveToConfig();

private slots:
    void NbFileHistoryValueChanged(int);
    void TailDelayValueChanged(int);

private:
    Ui::ucFileConfig *ui;
    QSettings *settings;
    int curNbFileHistory;
    int curTailDelay;

    void Init();
};

#endif // UCFILECONFIG_H
