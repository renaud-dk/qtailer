#ifndef UCDEFAULTHIGHLIGHT_H
#define UCDEFAULTHIGHLIGHT_H

#include "hightlight.h"

#include <QWidget>
#include <QSettings>
#include <QColorDialog>
#include <QMessageBox>

namespace Ui {
    class ucDefaultHighLight;
}

class ucDefaultHighLight : public QWidget
{
    Q_OBJECT

public:
    explicit ucDefaultHighLight(QWidget *parent = 0);
    ~ucDefaultHighLight();

    bool HasChanged();
    void SaveToConfig();

private slots:
    void ShowColorChooser();
    void AddClicked();
    void RemoveClicked();

private:
    Ui::ucDefaultHighLight *ui;
    bool hasChanged;
    QSettings *settings;
    QList<HightLight> *lstDefHighLight;
    QString currentColor;
    QString currentWord;

    void Init();
};

#endif // UCDEFAULTHIGHLIGHT_H
