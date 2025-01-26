#include <QCoreApplication>
#include <QTcpSocket>
#include <QTextStream>
#include <QDebug>
#include <QCommandLineParser>
#include "../Logger/myLogger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customMessageHandler);

    // Set up command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("TCP Client Application");
    parser.addHelpOption();
    parser.addVersionOption();

    // Add options for address and port
    QCommandLineOption addressOption(QStringList() << "a" << "address",
                                     "Server address to connect to.", "address", "127.0.0.1");
    parser.addOption(addressOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  "Port number to connect to.", "port", "12345");
    parser.addOption(portOption);

    // Parse command line arguments
    parser.process(a);

    // Get address and port from command line or use default values
    QString serverAddress = parser.value(addressOption);
    quint16 serverPort = parser.value(portOption).toUShort();
    qDebug() << serverAddress << "  " << serverPort;

    // Create a TCP socket object
    QTcpSocket socket;

    // Connect to the server
    socket.connectToHost(serverAddress, serverPort);

    if (!socket.waitForConnected()) {
        qCritical() << "Connection error:" << socket.errorString();
        return -1;
    }

    qDebug() << "Connected to server at address" << serverAddress << "and port" << serverPort;

    // Reading lines from the command line
    QTextStream inputStream(stdin);
    QString message;
    while (true) {
        //qDebug() << "Enter a string to send to the server (or 'exit' to quit):";
        message = inputStream.readLine();

        if (message == "exit") {
            break;
        }

        // Send the string to the server
        socket.write(message.toUtf8());
        socket.waitForBytesWritten();
        qDebug() << "Message sent:" << message;


        if(socket.waitForReadyRead(1000)) qDebug() << "Message received: " << socket.readAll();


    }

    // Close the socket
    socket.disconnectFromHost();
    socket.waitForDisconnected();

    qDebug() << "Connection closed.";

    return a.exec();
}
