#include "maindialog.h"
#include "ui_maindialog.h"
#include "constant.h"
#include "clientmanager.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    mServer(new QWebSocketServer("Pulas Server", QWebSocketServer::NonSecureMode, this)),
    mClientManager(new ClientManager(this)),
    mTray(new QSystemTrayIcon(this))
{
    ui->setupUi(this);
    connect(mServer, SIGNAL(newConnection()), SLOT(newConnection()));
    //directly run the server
    if (runServer())
        ui->labelInfo->setText("is running ...");
    else
        ui->labelInfo->setText("is failed to run!!!");

    setWindowTitle("Pulas - Javascript direct printing");

    connect(ui->pushAboutQt, SIGNAL(clicked(bool)), SLOT(aboutQtClicked()));
    connect(ui->pushQuit, SIGNAL(clicked(bool)), SLOT(quitClicked()));

    mTray->setIcon(QIcon(":/images/icon.png"));
    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    QPixmap px(":/images/icon.png");
    ui->image->setPixmap(px.scaledToHeight(150));
    ui->labelVersion->setText("Version " + QApplication::applicationVersion());
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    this->hide();
    mTray->show();
    event->ignore();
}

bool MainDialog::runServer()
{
    return mServer->listen(QHostAddress::Any, SERVER::port);
}

void MainDialog::newConnection()
{
    QWebSocket *sock = mServer->nextPendingConnection();
    while(sock) {
        mClientManager->addNewClient(sock);
        sock = mServer->nextPendingConnection();
    }
}

void MainDialog::aboutQtClicked()
{
    QMessageBox::aboutQt(this);
}

void MainDialog::quitClicked()
{
    qApp->exit();
}

void MainDialog::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    this->show();
}
