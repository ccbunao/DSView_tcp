#include "tcp_server.h"
#include <QtDebug>
#include "config/appconfig.h"

namespace pv
{

    tcp_control::tcp_control(QObject *parent)
    {
        Q_UNUSED(parent);
        help_msg =
            "==================================\n"
            "help      | show available commands\n"
            "start     | start capturing\n"
            "stop      | stop capturing\n"
            "instant   | capture immediately\n"
            "save file | save current data as file";
        connectClient = nullptr;
        tcpServer = new QTcpServer(this);
        connect(tcpServer, &QTcpServer::newConnection, this, &tcp_control::newConnectionSlot);
        bool ok = tcpServer->listen(QHostAddress::Any, 1999);
        if (!ok)
        {
            qDebug() << "start tcp server failure";
        }
    }

    tcp_control::~tcp_control()
    {
        if (connectClient != nullptr)
        {
            connectClient->disconnectFromHost();
            connectClient = nullptr;
        }
        if (tcpServer != nullptr)
        {
            tcpServer->close();
        }
    }

    void tcp_control::newConnectionSlot()
    {
        if (connectClient != nullptr)
        {
            QTcpSocket *unsup_port = tcpServer->nextPendingConnection();
            QString msg = "The software does not support multi-party control, the current port will be closed\n";
            unsup_port->write(msg.toUtf8());
            unsup_port->disconnectFromHost();
            return;
        }

        connectClient = tcpServer->nextPendingConnection();
        QString msg = "Welcom to DSView\n";
        connectClient->write(msg.toUtf8());
        connect(connectClient, &QTcpSocket::readyRead, this, &tcp_control::readDataSlot);
        connect(connectClient, &QTcpSocket::readyRead, this, &tcp_control::disconnectClientSlot);
    }
    void tcp_control::disconnectClientSlot()
    {
        if (connectClient == nullptr)
            return;
        if (connectClient->state() == QAbstractSocket::UnconnectedState)
        {
            connectClient->destroyed();
            connectClient = nullptr;
        }
    }

    void tcp_control::readDataSlot()
    {
        QByteArray buffer = connectClient->readAll();
        if (buffer.isEmpty())
        {
            return;
        }
        if (buffer == "\n")
        {
            return;
        }

        QString recvcmd = buffer;
        QStringList cmdlist = recvcmd.simplified().split(' ', QString::SkipEmptyParts);
        if (cmdlist.length() == 0)
        {
            return;
        }
        QString action = cmdlist.at(0);
        if (action == "start")
        {
            emit start();
        }
        else if (action == "stop")
        {
            emit stop();
        }
        else if (action == "instant")
        {
            emit instant();
        }
        else if (action == "save")
        {
            if (cmdlist.length() < 2)
            {
                show_message(QString("Missing saved filename"));
            }
            else
            {
                // show_message(QString("filename: %1").arg(cmdlist.at(1)));
                // printf("[tcp_control:readDataSlot]:%s",cmdlist.at(1));
                // emit save(cmdlist.at(1));

                QByteArray utf8Message = QString(cmdlist.at(1)).toUtf8();
                printf("[tcp_control:readDataSlot]: %s\n", utf8Message.constData());
                emit save(utf8Message);
            }
        }
        else if (action == "help")
        {
            show_message(help_msg);
        }
        else
        {
            show_message(QString("ubknow cmd: %1").arg(action));
        }
    }

    void tcp_control::show_message(QString msg)
    {
        if (connectClient != nullptr)
        {
            connectClient->write((msg + "\n").toUtf8());
        }
    }
}