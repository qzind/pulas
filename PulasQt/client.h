#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class QWebSocket;
class Printer;

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QWebSocket *sock, QObject *parent = 0);
    void shutdown();

private:
    QWebSocket *mSocket;
    Printer *mPrinter;

    void answer(int type, const QVariant &data, double token);

signals:
    void aboutToClose();

private slots:
    void disconnected();
    void textMessageRecieved(const QString &msg);
    void binaryMessageRecieved(const QByteArray &msg);
};

#endif // CLIENT_H
