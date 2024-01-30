#include "GSMModem.h"

QT_USE_NAMESPACE

GSMModem::GSMModem(QObject *parent) : QObject(parent)
{
    bSetCMGF=bSetUCS2=portState=bAT=waitingCOPS=false;
    timerCounter=counterAT=counterOPS=0;
    opsID=-1;
    opsName="";
    port = new QSerialPort(this);
    connect(port,SIGNAL(readyRead()),this,SLOT(readyRead()));
    ports = QSerialPortInfo::availablePorts();
    timerAT= new QTimer(this);
    timerAT->setInterval(1000);
    connect(timerAT,SIGNAL(timeout()),SLOT(slotTimer()));

}

void GSMModem::connectModem(QString name) {
    if(port->isOpen())
        port->close();
    if(!name.isEmpty()) port->setPortName(name);
    port->setBaudRate(115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    if (!port->open(QIODevice::ReadWrite)) {

        emit error("Ошибка открытия порта");
        return;
    }
    emit error("Порт открыт");
    timerAT->start();
}

void GSMModem::readyRead() {
    QByteArray data=port->readAll();
    //emit error(data);
    QString str;
    QStringList list;
    QTextStream stream(data);
    while(!stream.atEnd()) {
        str=stream.readLine();
        if(str=="OK") {
            bAT=true;
            continue;
        }
        if(str.startsWith("+CMGS:")) {
            delete listSMS[0];
            listSMS.removeAt(0);
        }
        if(str.startsWith("+COPS: (")) {
            list=str.mid(7).split("),(");
            foreach (QString str, list) {
                if(str.startsWith('('))str=str.mid(1);
                if(str.endsWith(')')) str=str.mid(0,str.length()-1);
                QStringList params=str.split(',');
                if(params.count()==5) {
                    if(opsID==params[3].replace("\"","").toInt()) {
                        opsName=params[2].replace("\"","");
                        emit operatorName(opsName);
                        waitingCOPS=false;
                        break;
                    }
                }
            }
            return;
        }
        if(str.startsWith("+COPS:")) {
            list=str.mid(7).split(',');
            if(list.count()==1) {
                QByteArray data="at+cops=0,2,\"25001\"\xd";
                //emit error(data);
                port->write(data,20);
                return;
            }
            opsID=list[2].replace("\"","").toInt();
            emit operatorName(QString::number(opsID));
        }
        if(str.startsWith("+CSQ:")) {
            list=str.mid(6).split(',');
            emit signalRower(list[0].toInt());

        }
    }
}

GSMModem::~GSMModem(){
    port->close();
}

void GSMModem::slotTimer() {
    if(portState!=port->isOpen()) {
        portState=port->isOpen();
        emit portOpen(portState);
    }
    timerCounter++;
    if(!bAT) {
        if(counterAT>3) {
            counterAT=0;
            connectModem();
        }
        QByteArray data="at\xd";
        qDebug()<<data;
        port->write(data,3);
        counterAT++;
        return;
    }
    if(opsID==-1) {
        QByteArray data="at+cops?\xd";
        qDebug()<<data;
        port->write(data,9);
        return;
    }
    if(opsName.isEmpty()) {
        if(!waitingCOPS) {
            counterOPS=0;
            waitingCOPS=true;
            QByteArray data="at+cops=?\xd";
            qDebug()<<data;
            port->write(data,10);
        }
        else {
            if(counterOPS>50)
                waitingCOPS=false;
            qDebug()<<"counterOPS="<<(counterOPS++);
        }
        return;
    }
    if(!bSetUCS2) {
        QByteArray data="at+cscs=\"UCS2\"\xd";
        qDebug()<<data;
        port->write(data,15);
        bSetUCS2=true;
        return;

    }
    if(!bSetCMGF) {
        QByteArray data="AT+CMGF=0\xd";
        qDebug()<<data;
        port->write(data,10);
        bSetCMGF=true;
        return;

    }
    if(timerCounter%5==0) {
        QByteArray data="at+csq\xd";
        qDebug()<<data;
        port->write(data,10);
        return;
    }
    if(!listSMS.isEmpty()) {
        if(listSMS[0]->isSending && QDateTime::currentDateTime().toTime_t()-listSMS[0]->timeSending.toTime_t()>10)listSMS[0]->isSending=false;
        if(listSMS[0]->isSending) return;
        QString data="AT+CMGS=%1";
        data=data.arg(listSMS[0]->text.length()/2-1);
        //emit error("Send SMS: "+data);
        port->write(data.toLocal8Bit().append("\xd"),data.length()+1);
        port->waitForReadyRead(1000);
        //emit error("Send SMS: "+listSMS[0]->text.toLocal8Bit());
        listSMS[0]->isSending=true;
        listSMS[0]->timeSending=QDateTime::currentDateTime();
        port->write(listSMS[0]->text.toLocal8Bit().append("\x1A"),listSMS[0]->text.length()+1);
    }
}

void GSMModem::sendSMS(QString phone, QString text) {
    QChar ch;
    int phoneLength;
    if(phone.startsWith("8"))
        phone="7"+phone.mid(1);
    if(phone.startsWith("+"))
        phone=phone.mid(1);
    phoneLength=phone.length();

    if(phone.length()%2==1)
        phone+="F";
    for(int i=0;i<phone.length();i+=2) {
        ch=phone[i];
        phone[i]=phone[i+1];
        phone[i+1]=ch;
    }
    //                                                                                 !
    phone="001100"+QString::number(phoneLength,16).rightJustified(2,'0')+"91"+phone+"000800"+QString::number(text.size()*2,16).rightJustified(2,'0');
    QByteArray d((const char*) (text.utf16()), text.size() * 2);
    for(int i=0;i<d.length();i+=2)
        phone=phone+QString::number(d.at(i+1),16).rightJustified(2,'0')+QString::number(d.at(i),16).rightJustified(2,'0');
    //emit error("Add to list: AT+CMGS="+QString::number(phone.length()/2-1)+" "+phone.toUpper());
    listSMS.append(new SMS(phone.toUpper()));
}
