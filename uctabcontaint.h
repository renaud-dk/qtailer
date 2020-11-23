#ifndef UCTABCONTAINT_H
#define UCTABCONTAINT_H

#include "qtailthread.h"
#include "selecthighlight.h"
#include "csession.h"

#include <QWidget>
#include <QUrl>
#include <QFileInfo>
#include <QDropEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDragEnterEvent>
#include <QWaitCondition>
#include <QSettings>
#include <QDateTime>
#include <QProcess>
#include <QStringList>

namespace Ui {
    class ucTabContaint;
}

/*!
 * \brief Main user control for tailing display.
 *
 * Main user control used in every tab. This component holds the tailing thread
 * and applies the user requested operation (word highlight,
 * file selection, ...). This class derived form the Qt class QWidget.
 */

class ucTabContaint : public QWidget
{
    Q_OBJECT

public:
    explicit ucTabContaint(int tabIndex, QWidget *parent = 0);
    explicit ucTabContaint(CSession session, QWidget *parent = 0);
    ~ucTabContaint();

    QString getFileName ();
    QString getFilePath ();
    CSession getSessionInfo();
    void setTabIndex (int);
    void StopTailing();
    void StartTailing();
    void ReloadConfig();

signals:
    void FileChanged(const QString &, const QString &, int);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);

private slots:
    void ActionButtonClicked();
    void FileOpenClicked(void);
    void FileSelected(QString fileName);
    void NewLineToDisplay (QStringList);
    void SelectFileChanged(QString);
    void ShowSelectHighLight(void);
    void OpenInEditor(void);
    void FilterCheckChange(int);
    void FilterCleared(QString);

private:
    Ui::ucTabContaint *ui;
    QTailThread *tailThread;
    QFileDialog *selectFileDialog;

    QFileInfo *fileInfo;

    QString previousFilePath;
    QStringList *lstLine;
    QStringList *lstFilteredLine;
    QList<HightLight> *lstDefHighlight;
    QList<HightLight> *lstCustHighlight;

    QSettings *settings;

    bool isStarted;
    bool firstRun;
    bool prevStartState;
    int tabIndex;

    void setStopView ();
    void setStartView();
    void Init();
    void Init(CSession);
    void ReFreshFileShow();
    void FilterText();
    void SaveToHistory (QString);
    void RestoreFileView();
};

#endif // UCTABCONTAINT_H
