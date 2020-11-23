#ifndef QTAILCONFIG_H
#define QTAILCONFIG_H

#include "ucgeneralconfig.h"
#include "ucfileconfig.h"
#include "uceditfilehistory.h"
#include "ucdefaulthighlight.h"

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QCoreApplication>

namespace Ui {
    class QTailConfig;
}

class QTailConfig : public QDialog
{
    Q_OBJECT

public:
    explicit QTailConfig(QWidget *parent = 0);
    ~QTailConfig();
    bool ConfigHasChanged();

private slots:
    void MenuSelected(QListWidgetItem *);
    void ButtonClicked(QAbstractButton*);

private:
    Ui::QTailConfig *ui;
    QString currentItem;
    bool reloadNeeded;

    void Init();
    void SetTab(QWidget*, QString);
    void GenerateDefaultConfig();
    bool CheckToSave();
    void SaveConfig();
};

#endif // QTAILCONFIG_H
