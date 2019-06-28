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

    processId = processGetPid("dro_client.exe");
    qDebug() << "PID: " << processId;
    ts << "PID: " << processId << '\r\n';
    DWORD errCode = GetLastError();
    qDebug() << errCode << FM(errCode);

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
    errCode = GetLastError();
    qDebug() << errCode << FM(errCode);
    qDebug() << "PH: " << processHandle;
    ts << "PH: " << processHandle << '\r\n';

    int fakeThreadStack = getFakeThreadStack(processHandle, processId);
    qDebug() << "FTS: " << fakeThreadStack;
    ts << "FTS: " << fakeThreadStack << '\r\n\r\n';

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
