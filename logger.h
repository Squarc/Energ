#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

class Logger
{
public:
    Logger(QString fileName);
    void Message(QString msg);
    void File(QString fileName, QByteArray data);
    void Post(QString fileName, QString post, QByteArray data);
    void Recive(QString fileName, QByteArray data);
private:
    QFile file;
};

#endif // LOGGER_H
