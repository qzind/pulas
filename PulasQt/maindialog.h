#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

class QWebSocketServer;
class ClientManager;

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private:
    Ui::MainDialog *ui;
    QWebSocketServer *mServer;
    ClientManager *mClientManager;

private slots:
    void runServer();
    void newConnection();
};

#endif // MAINDIALOG_H
