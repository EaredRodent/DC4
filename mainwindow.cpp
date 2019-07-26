#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWebEngineView *webView = new QWebEngineView();
    webView->page()->profile()->setHttpUserAgent("DSO_QuickLauncher");
    webView->hide();
    ui->verticalLayout->addWidget(webView);

    Bridge *bridge = new Bridge();

    QWebChannel *webChannel = new QWebChannel();
    QObject::connect(webView, SIGNAL(titleChanged(QString)), SLOT(setWindowTitleByEngine(QString)));
    webChannel->registerObject("bridge", bridge);
    webView->page()->setWebChannel(webChannel);

    QEventLoop el;
    QObject::connect(webView, SIGNAL(loadFinished(bool)), &el, SLOT(quit()));
    webView->load(QUrl(clientUrl));
    el.exec();
    if(!webView->page()->title().contains("DSO_Cam")) {
        QMessageBox::critical(0, "Error", "Network problems.");
        exit(0);
    }
    webView->show();

    DWORD processId;
    HANDLE processHandle;


    QFile file("init.log");
    file.open(QIODevice::Append);
    QTextStream ts(&file);

    DWORD errCode = 0;



    //*********************
    //    HANDLE hToken = NULL;
    //    TOKEN_PRIVILEGES tokenPriv;
    //    LUID luidDebug;
    //    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE)
    //    {
    //        if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug) != FALSE)
    //        {
    //            tokenPriv.PrivilegeCount           = 1;
    //            tokenPriv.Privileges[0].Luid       = luidDebug;
    //            tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    //            if(AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL) != FALSE)
    //            {
    //                // Always successful, even in the cases which lead to OpenProcess failure
    //                qDebug() << "SUCCESSFULLY CHANGED TOKEN PRIVILEGES" << endl;
    //            }
    //            else
    //            {
    //                qDebug() << "FAILED TO CHANGE TOKEN PRIVILEGES, CODE: " << GetLastError() << endl;
    //            }
    //        }
    //    }
    //    CloseHandle(hToken);
    //*********************


    // PID
    processId = processGetPid("dro_client.exe");

//    errCode = GetLastError();
//    if(errCode) {
//        QMessageBox::critical(0, "Error", FM(errCode));
//        exit(0);
//    }

    // OPEN PROC
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

//    errCode = GetLastError();
//    if(errCode) {
//        QMessageBox::critical(0, "OpenProcess", FM(errCode));
//        exit(0);
//    }

    int fakeThreadStack = getFakeThreadStack(processHandle, processId);
    qDebug() << "FTS: " << fakeThreadStack;

    QThread *chainThread = new QThread();
    Chain *chain = new Chain(processHandle, fakeThreadStack);
    QObject::connect(chainThread, SIGNAL(started()), chain, SLOT(init()));
    chain->moveToThread(chainThread);
    QObject::connect(bridge, SIGNAL(setValues(QJsonDocument)), chain, SLOT(getValues(QJsonDocument)));
    chainThread->start();
    QObject::connect(chain, SIGNAL(setValues(QJsonObject)), bridge, SLOT(getValues(QJsonObject)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowTitleByEngine(QString title) {
    setWindowTitle(title);
}
