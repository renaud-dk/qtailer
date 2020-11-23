#ifndef SELECTHIGHLIGHT_H
#define SELECTHIGHLIGHT_H

#include "hightlight.h"

#include <QDialog>
#include <QColorDialog>
#include <QMessageBox>

namespace Ui {
    class SelectHighLight;
}

class SelectHighLight : public QDialog
{
    Q_OBJECT

public:
    explicit SelectHighLight(QList<HightLight> lst, QWidget *parent = 0);
    ~SelectHighLight();
    QList<HightLight> * SelectItems();

private slots:
    void ShowColorChooser();
    void AddClicked();
    void RemoveClicked();

private:
    Ui::SelectHighLight *ui;
    QList<HightLight> *lstElt;
    QString currentColor;
    QString currentWord;

    void Init(QList<HightLight> lst);
};

#endif // SELECTHIGHLIGHT_H
