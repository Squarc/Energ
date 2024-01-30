#include "logger.h"

Logger::Logger(QString fileName)
{
    file.setFileName(fileName+".log");
    QDir dir("out");
    if(!dir.exists())dir.mkdir("out");
}

void Logger::Message(QString msg) {
    if(file.open(QFile::ReadWrite|QFile::Append)) {
        QTextStream stream(&file);

        stream<<(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")+" "+msg+"\r\n").toUtf8();
        file.close();
    }
}

void Logger::File(QString fileName, QByteArray data) {
    QFile file;
    fileName = "out/"+QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())+" "+fileName;
    Message("Сохранен файл '"+fileName+"'");
    file.setFileName(fileName);
    if(file.open(QFile::ReadWrite)) {
        file.write(data);
        file.close();
    }
}

void Logger::Post(QString fileName, QString post, QByteArray data) {
    QFile file;
    fileName = "out/"+QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())+" =>> "+fileName;
    Message("Сохранен файл '"+fileName+"'");
    file.setFileName(fileName);
    if(file.open(QFile::ReadWrite)) {
        file.write(post.toLocal8Bit()+"\r\n\r\n");
        file.write(data);
        file.close();
    }
}

void Logger::Recive(QString fileName, QByteArray data) {
    QFile file;
    fileName = "out/"+QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())+" <<= "+fileName;
    Message("Сохранен файл '"+fileName+"'");
    file.setFileName(fileName);
    if(file.open(QFile::ReadWrite)) {
        file.write(data);
        file.close();
    }
}




