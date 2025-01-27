#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket *socket, QObject *parent = nullptr)
        : QObject(parent), m_socket(socket)
    {
        connect(m_socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
        connect(m_socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
    }

private slots:
    void onReadyRead() {
        QByteArray data = m_socket->readAll();
        qDebug() << "Data received from client:" << data;

       // QByteArray data_res;
        for (auto &elem : data) {
            elem = processByte(elem);
           // data_res.push_back(processByte(elem));
        }
        m_socket->write(data);
    }

    void onDisconnected() {
        qDebug() << "Client disconnected.";
        QThread::currentThread()->quit();
    }

    uint8_t processByte(uint8_t byte) {
        uint8_t msb_mask = 0b10000000;
        uint8_t lsb_mask = 0b00000001;

        byte ^= msb_mask;
        byte ^= lsb_mask;

        byte ^= 0xA5;

        return byte;
    }

private:
    QTcpSocket *m_socket;
};

class EchoServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit EchoServer(int port, QObject *parent = nullptr) : QTcpServer(parent) {
        if (!this->listen(QHostAddress::Any, port)) {
            qDebug() << "Failed to start the server:" << errorString();
        } else {
            qDebug() << "Server started on port" << port;
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket *clientSocket = new QTcpSocket();

        if (clientSocket->setSocketDescriptor(socketDescriptor)) {
            qDebug() << "Client connected:" << clientSocket->peerAddress().toString();

            ClientHandler *handler = new ClientHandler(clientSocket);
            QThread *clientThread = new QThread();
            clientSocket->moveToThread(clientThread);
            handler->moveToThread(clientThread);

            connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);
            connect(clientThread, &QThread::finished, handler, &ClientHandler::deleteLater);
            connect(clientThread, &QThread::finished, clientSocket, &QTcpSocket::deleteLater);

            clientThread->start();
        } else {
            delete clientSocket;
        }
    }
};
#endif // SERVER_H
