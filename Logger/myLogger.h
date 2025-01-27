#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>
#include <QDateTime>
#include <QCoreApplication>
#include <iostream>

#ifdef Q_OS_WIN
#ifdef LOGGER_LIBRARY
#define LOGGER_EXPORT Q_DECL_EXPORT
#else
#define LOGGER_EXPORT Q_DECL_IMPORT
#endif
#else
#define LOGGER_EXPORT
#endif

class LOGGER_EXPORT FileLogger : public QObject {
public:
    explicit FileLogger(const QString &filePath);
    ~FileLogger();
public Q_SLOTS:
    void logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

Q_SIGNALS:
    void signallogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    QFile logFile;
};

void LOGGER_EXPORT customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // MYLOGGER_H
