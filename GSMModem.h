#ifndef GSMMODEM_H
#define GSMMODEM_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QList>
#include <QDateTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SMS
{
public:
    QString text;
public:
    bool isSending;
    QDateTime timeSending;
    SMS(QString text) {
        this->text=text;
        isSending=false;
        timeSending=QDateTime::fromTime_t(0);
    }
};

class GSMModem : public QObject
{
    Q_OBJECT
public:
    QList<SMS*> listSMS;
    int timerCounter;
    int opsID;
    QString opsName;
    bool bSetUCS2,bAT,waitingCOPS,portState,bSetCMGF;
    int counterAT,counterOPS;
    QTimer *timerAT;
    QList<QSerialPortInfo> ports;
    QSerialPort *port;
    explicit GSMModem(QObject *parent = nullptr);
    ~GSMModem();
    void connectModem(QString name="");
    void sendSMS(QString phone,QString text);
signals:
    void error(QString err);
    void signalRower(int value);
    void operatorName(QString name);
    void portOpen(bool state);
public slots:
    void readyRead();
    void slotTimer();
};

#endif // GSMMODEM_H
