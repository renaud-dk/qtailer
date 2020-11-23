#ifndef UCGENERALCONFIG_H
#define UCGENERALCONFIG_H

#include <QWidget>
#include <QSettings>
#include <QColorDialog>
#include <QFileDialog>

namespace Ui {
    class ucGeneralConfig;
}

class ucGeneralConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ucGeneralConfig(QWidget *parent = 0);
    ~ucGeneralConfig();

    bool HasChanged();
    void SaveToConfig();

private:
    Ui::ucGeneralConfig *ui;
    QFileDialog *selectFileDialog;
    QSettings *settings;

    int curMaxTabNumber;
    int curLineToDisplay;
    bool enableSession;
    QString curStartColor;
    QString curStopColor;
    QString curTextEditor;


    void Init();

private slots:
    void SetStartColor();
    void SetStopColor();
    void FileSelectClicked();
    void FileSelected(QString);
    void MaxTabValueChanged(int);
    void LineToDisplayValueChanged(int);
    void EnableSessionChanged(int);
};

#endif // UCGENERALCONFIG_H
