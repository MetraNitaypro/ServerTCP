# Название библиотеки
TEMPLATE = lib
CONFIG += dll
DEFINES += LOGGER_LIBRARY
# Исходные файлы
SOURCES += \
    myLogger.cpp

# Заголовочные файлы
HEADERS += \
    myLogger.h

# Подключаемые модули
QT += core

# Путь для сборки DLL
DESTDIR = ../lib
