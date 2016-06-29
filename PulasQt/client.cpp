#include "client.h"
#include "constant.h"
#include "printer.h"

#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Client::Client(QWebSocket *sock, QObject *parent) :
    QObject(parent),
    mSocket(sock),
    mPrinter(new Printer(this))
{
    connect(mSocket, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(QString)), SLOT(textMessageRecieved(QString)));
    connect(mSocket, SIGNAL(binaryMessageReceived(QByteArray)), SLOT(binaryMessageRecieved(QByteArray)));
}

void Client::shutdown()
{
    mSocket->close();
    emit aboutToClose();
    deleteLater();
}

void Client::answer(int type, const QVariant &data, double token)
{
    QVariantMap m;
    m["type"] = QVariant(type);
    m["data"] = data;
    m["token"] = QVariant(token);
    mSocket->sendTextMessage(QJsonDocument::fromVariant(QVariant(m)).toJson(QJsonDocument::Compact));
}

void Client::disconnected()
{
    shutdown();
}

void Client::textMessageRecieved(const QString &msg)
{
    //message will always be json
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8(), &parseError);
    if(parseError.error != QJsonParseError::NoError)
        return;

    const QVariantMap json = doc.object().toVariantMap();
    int type = json.value("type", QVariant(MSG_TYPE::MSG_UNKNOWN)).toInt();
    double token = json.value("token", QVariant(0.0f)).toDouble();
    QVariant v;
    switch(type) {
        case MSG_TYPE::MSG_LIST_PRINT :
        {
            v = mPrinter->getListPrinter();
        } break;
        case MSG_TYPE::MSG_GET_DEFAULT_PRINT:
        {
            v = mPrinter->getDefaultPrinterName();
        } break;
        case MSG_TYPE::MSG_SELECT_PRINT:
        {
            v = mPrinter->setPrinter(json.value("print", mPrinter->getDefaultPrinterName()).toString());
        } break;
        case MSG_TYPE::MSG_GET_CURRENT_PRINT:
        {
            v = mPrinter->getCurrentPrinter();
        } break;
        case MSG_TYPE::MSG_SETTING_PRINT:
        {
            v = mPrinter->settingPrinter(json.value("setting", QVariant()));
        } break;
        case MSG_TYPE::MSG_PRINT:
        {
            v = mPrinter->print(json.value("data", QVariant()));
        } break;
        case MSG_TYPE::MSG_GET_SUPPORTED_RESOLUTION:
        {
            v = mPrinter->getSupportedResolution();
        } break;
    }
    answer(type, v, token);
}

void Client::binaryMessageRecieved(const QByteArray &msg)
{
    Q_UNUSED(msg);
}

