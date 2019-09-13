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

    //*********************
    // !!! WIN 10 ONLY !!!
    //*********************
//            HANDLE hToken = NULL;
//            TOKEN_PRIVILEGES tokenPriv;
//            LUID luidDebug;
//            if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE)
//            {
//                if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug) != FALSE)
//                {
//                    tokenPriv.PrivilegeCount           = 1;
//                    tokenPriv.Privileges[0].Luid       = luidDebug;
//                    tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//                    if(AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL) != FALSE)
//                    {
//                        // Always successful, even in the cases which lead to OpenProcess failure
//                        qDebug() << "SUCCESSFULLY CHANGED TOKEN PRIVILEGES" << endl;
//                    }
//                    else
//                    {
//                        qDebug() << "FAILED TO CHANGE TOKEN PRIVILEGES, CODE: " << GetLastError() << endl;
//                    }
//                }
//            }
//            CloseHandle(hToken);
    //*********************

    QThread *chainThread = new QThread();
    Chain *chain = new Chain(version);
    QObject::connect(chainThread, SIGNAL(started()), chain, SLOT(init()));
    chain->moveToThread(chainThread);
    QObject::connect(bridge, SIGNAL(toChain(QVariantMap)), chain, SLOT(fromBridge(QVariantMap)));
    chainThread->start();
    QObject::connect(chain, SIGNAL(toBridge(QVariantMap)), bridge, SLOT(fromChain(QVariantMap)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowTitleByEngine(QString title) {
    setWindowTitle(title);
}
