#ifndef QTAILERMAIN_H
#define QTAILERMAIN_H

#include "uctabcontaint.h"
#include "about.h"
#include "qtailconfig.h"
#include "constant.h"
#include "csession.h"
#include "sessionchooser.h"
#include "sessionname.h"

#include <QSettings>
#include <QMainWindow>
#include <QPushButton>
#include <QFileInfo>
#include <QDesktopWidget>

namespace Ui {
    class QTailerMain;
}

class QTailerMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTailerMain(QWidget *parent = 0);
    ~QTailerMain();

    // Custom methods.
    QWidget * getTabPage(int index);

private:
    // Fields
    Ui::QTailerMain *ui;
    QPushButton *addTabButton;
    int tabCounter;
    QSettings *settings;
    QList<CSession> _lstSession;
    bool isNewSession;
    QString currentSessionName;

    // Methods
    void Init(void);
    void AddTabPage(CSession);
    void closeEvent(QCloseEvent *event);

private slots:
    void addTabbedButtonClicked();
    void closeTabButtonClicked(int index);
    void selectedFileChanged(QString, QString, int);
    void MenuBarClicked(QAction *);
    void TabChange(int);
};

#endif // QTAILERMAIN_H
