#ifndef DSVIEW_PV_TCP_SERVER_H
#define DSVIEW_PV_TCP_SERVER_H

#include <stdint.h>
#include <string>
#include <thread>
#include <QObject>
#include <QtNetwork>

namespace pv
{

    class tcp_control : public QObject
    {
        Q_OBJECT
    public:
        explicit tcp_control(QObject *parent = 0);
        ~tcp_control();
        void show_message(QString msg);

    private:
        QTcpServer *tcpServer;
        QTcpSocket *connectClient;

        QString help_msg;
    public slots:
        void newConnectionSlot();
        void readDataSlot();
        void disconnectClientSlot();
    signals:
        void start(void);
        void stop(void);
        void instant(void);
        void save(QString);
    };
}
#endif