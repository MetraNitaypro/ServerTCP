#include "myLogger.h"

FileLogger::FileLogger(const QString &filePath) {
    connect(this, &FileLogger::signallogMessage, this, &FileLogger::logMessage);
    logFile.setFileName(filePath);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open file for logging";
    }
}

FileLogger::~FileLogger() {
    if (logFile.isOpen()) {
        logFile.close();
    }
}

void FileLogger::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (!logFile.isOpen()) return;

    QTextStream out(&logFile);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QString logLevel;
    switch (type) {
    case QtDebugMsg: logLevel = "DEBUG"; break;
    case QtInfoMsg: logLevel = "INFO"; break;
    case QtWarningMsg: logLevel = "WARNING"; break;
    case QtCriticalMsg: logLevel = "CRITICAL"; break;
    case QtFatalMsg: logLevel = "FATAL"; break;
    }

    out << timestamp << " [" << logLevel << "] "
        << context.category << ": " << msg << "\n";
    out.flush();
    QString logMessage = timestamp + " [" + logLevel + "] " + context.category + ": " + msg + "\n";

    // Log to file
    out << logMessage << "\n";
    out.flush();

    // Log to console with appropriate levels
    std::cout << logMessage.toStdString();
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static FileLogger logger("application.log");
   // logger.logMessage(type, context, msg);
    emit logger.signallogMessage(type, context, msg);
}
