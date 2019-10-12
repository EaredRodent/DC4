#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QThread>
#include <QEventLoop>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include <windows.h>
#include <processthreadsapi.h>

#include "chain.h"
#include "bridge.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
#ifdef QT_DEBUG
    QString clientUrl = "http://dso-client/private/dso-cam";
#endif
#ifndef QT_DEBUG
    QString clientUrl = "https://dso.earedrodent.com/private/dso-cam";
//    QString clientUrl = "http://dso-client/private/dso-cam";
#endif
    QString coreVersion = "4.1.2";
public slots:
    void setWindowTitleByEngine(QString title);
};

#endif // MAINWINDOW_H
