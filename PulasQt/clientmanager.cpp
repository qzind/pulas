#include "clientmanager.h"
#include "client.h"

#include <QWebSocket>
#include <QDebug>

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{
}

void ClientManager::addNewClient(QWebSocket *sock)
{
    Client *c = new Client(sock, this);
    mClientList.push_back(c);
    connect(c, SIGNAL(aboutToClose()), SLOT(clientClosed()));
}

void ClientManager::shutdown()
{
    foreach (Client *c, mClientList) {
        c->shutdown();
    }
}

void ClientManager::clientClosed()
{
    Client *c = static_cast<Client*>(QObject::sender());
    mClientList.removeOne(c);
}

