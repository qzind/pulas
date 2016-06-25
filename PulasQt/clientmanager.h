#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QList>

class QWebSocket;
class Client;

class ClientManager : public QObject
{
    Q_OBJECT
public:
    ClientManager(QObject *parent = 0);
    void addNewClient(QWebSocket *sock);
    void shutdown();

private:
    QList<Client*> mClientList;

private slots:
    void clientClosed();
};

#endif // CLIENTMANAGER_H
