#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QThread>
#include <QEventLoop>
#include <windows.h>
#include <processthreadsapi.h>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "system.h"
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
    QString clientUrl = "http://dso-client/dso-cam";
#endif
#ifndef QT_DEBUG
//    QString clientUrl = "https://dso.earedrodent.com/dso-cam";
    QString clientUrl = "http://dso-client/dso-cam";
#endif
    QString version = "4.0.0";
public slots:
    void setWindowTitleByEngine(QString title);
};

#endif // MAINWINDOW_H
