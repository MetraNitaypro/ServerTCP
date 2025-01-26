#include <QCoreApplication>
#include "server.h"
#include <QCommandLineParser>
#include "../Logger/myLogger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customMessageHandler);
    // Настроим парсер командной строки
    QCommandLineParser parser;
    parser.setApplicationDescription("TCP Client Application");
    parser.addHelpOption();
    parser.addVersionOption();

    // Добавим опцию для порта
    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  "Port number to connect to.", "port", "12345");
    parser.addOption(portOption);

    // Парсим аргументы командной строки
    parser.process(a);

    // Получаем порт из командной строки или используем значение по умолчанию
    quint16 port = parser.value(portOption).toUShort();
    // Создаем сервер
    EchoServer server(port);

    return a.exec();
}


