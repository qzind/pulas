#include "maindialog.h"
#include "ui_maindialog.h"
#include "constant.h"
#include "clientmanager.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    mServer(new QWebSocketServer("Pulas Server", QWebSocketServer::NonSecureMode, this)),
    mClientManager(new ClientManager(this))
{
    ui->setupUi(this);
    connect(mServer, SIGNAL(newConnection()), SLOT(newConnection()));
    //directly run the server
    runServer();

    setWindowTitle("Pulas - Javascript direct printing");
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::runServer()
{
    mServer->listen(QHostAddress::Any, SERVER::port);
}

void MainDialog::newConnection()
{
    QWebSocket *sock = mServer->nextPendingConnection();
    while(sock) {
        mClientManager->addNewClient(sock);
        sock = mServer->nextPendingConnection();
    }
}

