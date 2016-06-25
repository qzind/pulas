#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>

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
    void closeEvent(QCloseEvent * event);

private:
    Ui::MainDialog *ui;
    QWebSocketServer *mServer;
    ClientManager *mClientManager;
    QSystemTrayIcon *mTray;

private slots:
    bool runServer();
    void newConnection();
    void aboutQtClicked();
    void quitClicked();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // MAINDIALOG_H
