#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    testingHorn=false;
    modem = new GSMModem(this);
    foreach (QSerialPortInfo info, modem->ports) {
        ui->cbPort->addItem(info.portName()+" "+info.description()+(info.isBusy()?" (занят)":""),QVariant(info.portName()));
        if(info.description()=="HUAWEI Mobile Connect - Modem")
            ui->cbPort->setCurrentIndex(ui->cbPort->count()-1);
        else if(info.description()=="Mobile Connect - Modem")
            ui->cbPort->setCurrentIndex(ui->cbPort->count()-1);

    }
    logger = new Logger(QString("data"));
    connect(modem,SIGNAL(operatorName(QString)),SLOT(operatorName(QString)));
    connect(modem,SIGNAL(signalRower(int)),SLOT(signalRower(int)));
    connect(modem,SIGNAL(portOpen(bool)),SLOT(portState(bool)));
    connect(modem,SIGNAL(error(QString)),SLOT(slotMessage(QString)));
    languages.clear();
    server = new QTcpServer(this);
    key_control=key_shift=key_alt=false;
    player = new QMediaPlayer(this);
    bErrorE=bErrorE1=bErrorE2=bErrorE3=bErrorW=bErrorW1=bErrorW2=bErrorW3=false;
    player->setVolume(100);
    unsigned char data[]={
        220, 137, 45, 186, 85, 168, 118, 37, 133, 16, 150, 3, 109, 19, 160, 234,
        216, 154, 232, 180, 215, 201, 22, 57, 110, 225, 120, 86, 141, 99, 155, 56,
        66, 116, 29, 217, 45, 132, 187, 200, 52, 49, 178, 44, 118, 104, 71, 1,
        133, 206, 156, 168, 208, 95, 205, 166, 97, 86, 61, 164, 197, 76, 41, 182,
        80, 163, 126, 227, 130, 38, 239, 225, 195, 232, 75, 88, 118, 6, 143, 44,
        22, 139, 61, 222, 32, 32, 204, 106, 195, 82, 68, 75, 145, 225, 240, 112,
        223, 248, 224, 62, 212, 60, 247, 159, 255, 7, 236, 248, 132, 5, 150, 94,
        121, 251, 102, 3, 143, 237, 52, 241, 244, 94, 62, 4, 122, 239, 109, 32,
        32, 32, 240, 54, 217, 72, 217, 196, 145, 47, 62, 142, 150, 67, 217, 107,
        39, 90, 128, 247, 64, 11, 186, 165, 99, 98, 177, 11, 152, 215, 79, 123,
        179, 129, 159, 218, 214, 135, 124, 180, 56, 216, 106, 136, 37, 29, 191, 99,
        69, 234, 8, 178, 33, 61, 167, 88, 225, 33, 192, 253, 92, 176, 87, 169,
        76, 68, 178, 147, 221, 229, 6, 52, 114, 6, 78, 158, 160, 111, 125, 20,
        121, 121, 215, 209, 17, 128, 157, 45, 58, 150, 233, 224, 145, 117, 218, 165,
        190, 183, 241, 5, 248, 74, 187, 105, 200, 30, 175, 121, 18, 33, 90, 164,
        77, 108, 185, 254, 25, 190, 229, 77, 241, 43, 246, 92, 64, 14, 36, 150,
        148, 216, 254};
    XORArray=QByteArray((char*)data,259);
    HAZ_Sync=GEN1_Sync=GEN2_Sync=GEN3_Sync=GEN1_Pow=GEN2_Pow=GEN3_Pow= -1;
    GEN1_Temp=GEN2_Temp=GEN3_Temp=0;
    secondCounter=0;
    warningTimer = new QTimer(this);
    connect(warningTimer,SIGNAL(timeout()),this,SLOT(warningTimer_elapsed()));
    warningTimer->setInterval(30000);
    warningTimer->setSingleShot(false);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_elapsed()));
    timer->setInterval(1000);
    timer->setSingleShot(false);
    //connect(timer,SIGNAL(timeout()),this,SLOT(timer_elapsed()));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(playerStateChanged(QMediaPlayer::State)));

    this -> setTrayIconActions();
    this -> showTrayIcon();
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    manager = new QNetworkAccessManager(this);
    //connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(manager_finished(QNetworkReply*)));
    getUsersData=getAlarmTypesData=getAlarmGroupsData=sessionGUID="";

    req= new QNetworkRequest(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/InetPro.asmx"));
    req->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/xml; charset=utf-8"));
    req->setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1; Trident/7.0; rv:11.0) like Gecko");
    timer->start();

    logger->Message("Старт программы");
    if(server->listen(QHostAddress::Any,8081)) {
        logger->Message("Слушаем порт 8081");
        connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    }
    else
        logger->Message("Ошибка открытия порта 8081");


    on_pbConnectModem_clicked();
    player->setMedia(QUrl("./Alarm3.wav"));
    player->setVolume(100);
    player->setPosition(0);
    player->play();
}

void MainWindow::slotMessage(QString message) {
    logger->Message(message);
}

bool MainWindow::event(QEvent *event){
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        quint32 key=keyEvent->nativeVirtualKey();
        if (key == 17)
            key_control=true;
        else if(key == 16)
            key_shift=true;
        else if(key == 18)
            key_alt=true;

    }
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = (QKeyEvent *)event;
        quint32 key=keyEvent->nativeVirtualKey();
        if (key == 17)
            key_control=false;
        else if(key == 16)
            key_shift=false;
        else if(key == 18)
            key_alt=false;
    }
    return QWidget::event(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!key_control||!key_shift||!key_alt)
        event->ignore();
}

MainWindow::~MainWindow()
{
    logger->Message("Завершение программы");
    delete logger;
    delete req;
    delete ui;
}

void MainWindow::timer_elapsed(){
    if(languages.isEmpty())
        readLanguages();
    else if(getUsersData.isEmpty())
        getUsers();
    else if(getAlarmTypesData.isEmpty())
        getAlarmTypes();
    else if(getAlarmGroupsData.isEmpty())
        getAlarmGroups();
    else if(sessionGUID.isEmpty())
        getSessionGUID();
    else
        registerSignals();
    secondCounter++;
    if(secondCounter==10) {
        secondCounter=0;
        getWFEvents();
    }
}

void MainWindow::operatorName(QString name) {
    ui->lblOperator->setText(name);
}

void MainWindow::signalPower(int value) {
    if(value==0)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect0.png"));
    if(value>0 && value<=6)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect1.png"));
    if(value>6 && value<=13)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect2.png"));
    if(value>13 && value<=18)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect3.png"));
    if(value>18 && value<=24)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect4.png"));
    if(value>24 && value<=31)
        ui->lblSignalPower->setPixmap(QPixmap(":/connect5.png"));
}

void MainWindow::portState(bool state) {
    ui->pbConnectModem->setEnabled(!state);
}

void MainWindow::newConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    socket->open(QTcpSocket::ReadWrite);
    QTextStream stream(socket);
    stream<<HAZ_Sync<<"|"<<GEN1_Sync<<"|"<<GEN2_Sync<<"|"<<GEN3_Sync<<"|"<<GEN1_Pow<<"|"<<GEN2_Pow<<"|"<<GEN3_Pow<<"|"<<GEN1_Temp<<"|"<<GEN2_Temp<<"|"<<GEN3_Temp;
    socket->close();
}

void MainWindow::warningTimer_elapsed() {
    if(bErrorW && !ui->cbDisableWarnings->isChecked()) {
        testingHorn=false;
        if(player->state()==QMediaPlayer::PausedState || player->state()==QMediaPlayer::StoppedState) {
            player->setMedia(QUrl("./Alarm3.wav"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }
    }
}

void MainWindow::playerStateChanged(QMediaPlayer::State s) {
    //logger->Message(QString::number(s));
    if(s==QMediaPlayer::PausedState || s==QMediaPlayer::StoppedState) {
        if(HAZ_Sync==0 && !ui->cbDisableSync->isChecked()) {
            testingHorn=false;
            player->setMedia(QUrl("./sirena.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }
        else if(bErrorE && !ui->cbDisableErrors->isChecked()) {
            testingHorn=false;
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }
        if(warningTimer->isActive() && !bErrorW)
            warningTimer->stop();
        /*else if(bErrorW && !ui->cbDisableWarnings->isChecked()) {
            testingHorn=false;
            player->setMedia(QUrl("./Alarm3.wav"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }*/
    }
    //if(HAZ_Sync==-1&&!testingHorn&&!bErrorE&&!bErrorW) {
        //player->stop();
        //logger->Message("Отключение сирены");
    //}
}

void MainWindow::showTrayIcon() {
    // Создаём экземпляр класса и задаём его свойства...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/icon.png");
    trayIcon -> setIcon(trayImage);
    trayIcon -> setContextMenu(trayIconMenu);

    // Подключаем обработчик клика по иконке...
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Выводим значок...
    trayIcon -> show();
}


void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();

            break;
        default:
            break;
    }
}

void MainWindow::setTrayIconActions() {
    // Setting actions...
    minimizeAction = new QAction("Hide", this);
    restoreAction = new QAction("Normalize", this);
    quitAction = new QAction("Exit", this);

    // Connecting actions to slots...
    connect (minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Setting system tray's icon menu...
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction (minimizeAction);
    trayIconMenu -> addAction (restoreAction);
    trayIconMenu -> addAction (quitAction);
}

void MainWindow::changeEvent(QEvent *event) {
    QMainWindow::changeEvent(event);
}

void MainWindow::getSessionGUID() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><Connect xmlns:d3p1=\"http://schemas.datacontract.org/2004/07/WF2006Silverlight.WcsInetPro\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.webfactory-world.de/\" /></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/InetPro.asmx"));
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(sessionGUID_finished()));
}
void MainWindow::sessionGUID_finished() {
    QXmlStreamReader *reader = new QXmlStreamReader(((QNetworkReply*)sender())->readAll());
    int connectionResult=0;

    while(!reader->atEnd()) {
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="ConnectResult") {
            token = reader->readNext();
            if(token==QXmlStreamReader::Characters)
                connectionResult=reader->text().toString().toInt();
        }
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="cryptedSessionGUID") {
            token = reader->readNext();
            if(token==QXmlStreamReader::Characters)
                base65=reader->text().toString();
        }
    }
    sessionGUID=wfDecode(base65).toUtf8();
    //ui->txtPOSTRez->append(sessionGUID);
    if(connectionResult==0) {
        registerSignals();
    }
    delete reader;
    delete sender();
}

void MainWindow::registerSignals() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><RegisterSignals xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.webfactory-world.de/\"><sessionGUID>###sessionGUID###</sessionGUID><signalNames>"
                                               "<string>ZAS Netz Leistungsschalter Rückmeldung</string>"
                                               "<string>GEN1_MFR3_PPU Generator Wirkleistung</string>" //мощность 1
                                               "<string>GEN2_MFR3_PPU Generator Wirkleistung</string>" //мощность 2
                                               "<string>GEN3_MFR3_PPU Generator Wirkleistung</string>" //мощность 3
                                               "<string>GEN1_HASanKunde Generator Leistungsschalter Rückmeldung</string>"
                                               "<string>GEN2_HASanKunde Generator Leistungsschalter Rückmeldung</string>"
                                               "<string>GEN3_HASanKunde Generator Leistungsschalter Rückmeldung</string>"
                                               "<string>GEN1_Messwerte T404 Raumluft</string>"
                                               "<string>GEN2_Messwerte T404 Raumluft</string>"
                                               "<string>GEN3_Messwerte T404 Raumluft</string>"
                                               "<string>GEN1_HASanKunde Läuft Rückmeldung</string>"
                                               "<string>GEN2_HASanKunde Läuft Rückmeldung</string>"
                                               "<string>GEN3_HASanKunde Läuft Rückmeldung</string>"
                                               "<string>GEN1_HASanKunde Notstop Rückmeldung</string>"
                                               "<string>GEN2_HASanKunde Notstop Rückmeldung</string>"
                                               "<string>GEN3_HASanKunde Notstop Rückmeldung</string>"
                                               "<string>GEN1_Logik Sammel-Störung</string>"
                                               "<string>GEN2_Logik Sammel-Störung</string>"
                                               "<string>GEN3_Logik Sammel-Störung</string>"
                                               "<string>GEN1_Logik Sammel-Warnung</string>"
                                               "<string>GEN2_Logik Sammel-Warnung</string>"
                                               "<string>GEN3_Logik Sammel-Warnung</string>"
                                               "</signalNames></RegisterSignals></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/InetPro.asmx"));
    postData.replace("###sessionGUID###",sessionGUID);
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(registerSignals_finished()));
}
void MainWindow::registerSignals_finished() {
    QXmlStreamReader *reader = new QXmlStreamReader(((QNetworkReply*)sender())->readAll());
    int RegisterSignalsResult=0;
    while(!reader->atEnd()) {
        QXmlStreamReader::TokenType token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="RegisterSignalsResult") {
            token = reader->readNext();
            if(token==QXmlStreamReader::Characters){
                RegisterSignalsResult=reader->text().toString().toInt();
                if(RegisterSignalsResult==-1) {
                    //ui->txtPOSTRez->append("Неуспешная регистрация сигналов");
                    getSessionGUID();
                }
                else if(RegisterSignalsResult==0) {
                    //ui->txtPOSTRez->append("Успешная регистрация сигналов");
                    parameters();
                }
            }
        }
    }
    delete reader;
    delete sender();
}

void MainWindow::parameters() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><GetUpdates xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.webfactory-world.de/\"><sessionGUID>###sessionGUID###</sessionGUID><requestID>710</requestID></GetUpdates></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/InetPro.asmx"));
    postData.replace("###sessionGUID###",sessionGUID);
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(getParameters_finished()));
}
void MainWindow::getParameters_finished() {
    QXmlStreamReader *reader = new QXmlStreamReader(((QNetworkReply*)sender())->readAll());
    list.clear();
    while(!reader->atEnd()) {

        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="signalNames") {
            do{
                token = reader->readNext();
                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="signalNames")break;
                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="string") {
                    token = reader->readNext();
                    if(token==QXmlStreamReader::Characters){
                        chars=reader->text().toString();
                        list.append(chars);
                    }
                    token = reader->readNext();
                }
            }while(true);
        }
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="signalValues") {
            for(int i=0;i<list.length();i++) {
                token = reader->readNext();
                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="anyType") {
                    token = reader->readNext();
                    if(token==QXmlStreamReader::Characters)
                        values(list[i],reader->text().toString());
                    token = reader->readNext();
                }
            }
        }
    }
    list.clear();
    delete reader;
    delete sender();
}

void MainWindow::readLanguages() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><ReadLanguages xmlns=\"http://www.ecom-webfactory.de/webfactory/webservices\" /></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/action.asmx"));
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(slotReadLanguages()));
}

void MainWindow::slotReadLanguages() {
    QXmlStreamReader *reader = new QXmlStreamReader(((QNetworkReply*)sender())->readAll());
    while(!reader->atEnd()) {
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="ReadLanguagesResponse") {
            do{
                token = reader->readNext();
                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="ReadLanguagesResponse")break;
                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="LanguageID") {
                    do{
                        token = reader->readNext();
                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="LanguageID")break;
                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="int") {
                            token = reader->readNext();
                            if(token==QXmlStreamReader::Characters){
                                LanguageRes *res=new LanguageRes();
                                res->LanguageID=reader->text().toInt();
                                languages.append(res);
                            }
                            token = reader->readNext();
                        }
                    }while(true);
                }
                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Description") {
                    int count=0;
                    do{
                        token = reader->readNext();
                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Description")break;
                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="string") {
                            token = reader->readNext();
                            if(token==QXmlStreamReader::Characters){
                                languages[count]->Description=reader->text().toString();
                                count++;
                            }
                            token = reader->readNext();
                        }
                    }while(true);
                }
                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Active") {
                    int count=0;
                    do{
                        token = reader->readNext();
                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Active")break;
                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="int") {
                            token = reader->readNext();
                            if(token==QXmlStreamReader::Characters){
                                languages[count]->Active=reader->text().toInt();
                                count++;
                            }
                            token = reader->readNext();
                        }
                    }while(true);
                }
                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Used") {
                    int count=0;
                    do{
                        token = reader->readNext();
                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Used")break;
                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="int") {
                            token = reader->readNext();
                            if(token==QXmlStreamReader::Characters){
                                languages[count]->Used=reader->text().toInt();
                                count++;
                            }
                            token = reader->readNext();
                        }
                    }while(true);
                }
            }while(true);
        }
    }
    foreach (LanguageRes *res, languages) {
        ui->cbLanguage->addItem(res->Description,QVariant(res->LanguageID));
        if(res->Active==1) ui->cbLanguage->setCurrentText(res->Description);
    }
    delete reader;
    delete sender();
}

void MainWindow::getUsers() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetUsers xmlns=\"http://www.ecom-webfactory.de/webfactory/webservices\" /></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/silverlighttools.asmx"));
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(slotGetUsers()));
}

void MainWindow::slotGetUsers() {
    qint64 start=0,end=0;
    QByteArray data=((QNetworkReply*)sender())->readAll();
    QXmlStreamReader *reader = new QXmlStreamReader(data);
    while(!reader->atEnd()) {
        end=reader->characterOffset();
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="GetUsersResult")
            start=reader->characterOffset();
        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="GetUsersResult")
            break;
    }
    getUsersData=data.mid(start,end-start);
    delete reader;
    delete sender();
}

void MainWindow::getAlarmTypes() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetAlarmTypes xmlns=\"http://www.ecom-webfactory.de/webfactory/webservices\"><languageID>"+QByteArray::number(ui->cbLanguage->currentData().toInt())+"</languageID></GetAlarmTypes></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/silverlighttools.asmx"));
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(slotGetAlarmTypes()));
}
void MainWindow::slotGetAlarmTypes() {
    qint64 start=0,end=0;
    QByteArray data=((QNetworkReply*)sender())->readAll();
    QXmlStreamReader *reader = new QXmlStreamReader(data);
    while(!reader->atEnd()) {
        end=reader->characterOffset();
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="GetAlarmTypesResult")
            start=reader->characterOffset();
        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="GetAlarmTypesResult")
            break;
    }

    getAlarmTypesData=QString(data).mid(start,end-start).toUtf8();
    //logger->File("getAlarmTypes+.xml",data);
    delete reader;
    delete sender();
}

void MainWindow::getAlarmGroups() {
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetAlarmGroups xmlns=\"http://www.ecom-webfactory.de/webfactory/webservices\"><languageID>"+QByteArray::number(ui->cbLanguage->currentData().toInt())+"</languageID></GetAlarmGroups></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/silverlighttools.asmx"));
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(slotGetAlarmGroups()));
}
void MainWindow::slotGetAlarmGroups() {
    qint64 start=0,end=0;
    QByteArray data=((QNetworkReply*)sender())->readAll();
    QXmlStreamReader *reader = new QXmlStreamReader(data);
    while(!reader->atEnd()) {
        end=reader->characterOffset();
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="GetAlarmGroupsResult")
            start=reader->characterOffset();
        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="GetAlarmGroupsResult")
            break;
    }
    getAlarmGroupsData=QString(data).mid(start,end-start).toUtf8();
    //logger->File("getAlarmGroups+.xml",data);
    delete reader;
    delete sender();
}

void MainWindow::getWFEvents() {
    if(getAlarmGroupsData.isEmpty()||getAlarmTypesData.isEmpty()) return;
    QByteArray postData="<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\
<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
<GetWFEvents xmlns=\"http://www.ecom-webfactory.de/webfactory/webservices\">\
<filter><AlarmFilter><Action>IncludeSome</Action>\
<EventResultFilter><Action>IncludeSome</Action><Items><WFEventResult>Successfull</WFEventResult>\
<WFEventResult>Unsuccessfull</WFEventResult></Items></EventResultFilter><EventTypeFilter>\
<Action>IncludeSome</Action><Items><WFEventType>AlarmOn</WFEventType><WFEventType>AlarmOff</WFEventType>\
<WFEventType>AlarmAcknowledged</WFEventType></Items></EventTypeFilter><AlarmTypeFilter>\
<Action>IncludeAll</Action><Items>"+getAlarmTypesData+"</Items></AlarmTypeFilter><AlarmGroupFilter>\
<Action>IncludeAll</Action><Items>"+getAlarmGroupsData+"</Items></AlarmGroupFilter><AlarmPriorityFilter>\
<Action>IncludeAll</Action><From>1</From><To>100</To></AlarmPriorityFilter></AlarmFilter>\
<ServerFilter><Action>IncludeSome</Action><EventResultFilter><Action>IncludeSome</Action>\
<Items><WFEventResult>Successfull</WFEventResult><WFEventResult>Unsuccessfull</WFEventResult></Items>\
</EventResultFilter><EventTypeFilter><Action>IncludeSome</Action><Items><WFEventType>ServerStarted</WFEventType>\
<WFEventType>ServerStopped</WFEventType></Items></EventTypeFilter></ServerFilter><UserFilter>\
<Action>IncludeSome</Action><EventResultFilter><Action>IncludeSome</Action><Items>\
<WFEventResult>Successfull</WFEventResult><WFEventResult>Unsuccessfull</WFEventResult></Items>\
</EventResultFilter><EventTypeFilter><Action>IncludeSome</Action><Items><WFEventType>UserLoggedIn</WFEventType>\
<WFEventType>UserLoggedOut</WFEventType><WFEventType>UserWroteSignal</WFEventType></Items></EventTypeFilter>\
<UserFilter><Action>IncludeAll</Action><Items>"+getUsersData+"</Items></UserFilter></UserFilter><LanguageID>"+
            QByteArray::number(ui->cbLanguage->currentData().toInt())+"</LanguageID>\
<TimePeriod><Start>"+QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</Start>\
<End>"+QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</End></TimePeriod>\
<MaxEvents>9999999</MaxEvents><TimeFilter><ChartMode>Online</ChartMode>\
<DateTimeStart>"+QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</DateTimeStart>\
<TimeStart>"+QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</TimeStart>\
<TimeEnd>"+QDateTime::currentDateTime().addSecs(600).toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</TimeEnd>\
<DateStart>"+QDateTime::currentDateTime().toString("yyyy-MM-ddT00:00:00").toUtf8()+"</DateStart>\
<DateEnd>"+QDateTime::currentDateTime().toString("yyyy-MM-ddT00:00:00").toUtf8()+"</DateEnd>\
<DateTimeEnd>"+QDateTime::currentDateTime().addSecs(600).toString("yyyy-MM-ddTHH:mm:ss").toUtf8()+"</DateTimeEnd>\
<DisplayRange><RangeType>Minutes</RangeType><RangeValue>10</RangeValue></DisplayRange><DisplayRangeStartOffset>\
<RangeType>Minutes</RangeType><RangeValue>0</RangeValue></DisplayRangeStartOffset><DisplayRangeLastValues>\
<RangeType>Days</RangeType><RangeValue>81</RangeValue></DisplayRangeLastValues></TimeFilter></filter></GetWFEvents></s:Body></s:Envelope>";
    req->setUrl(QUrl::fromUserInput("http://192.168.154.100/_WEBFACTORY/WebServices/silverlighttools.asmx"));
    //logger->File("getWFEvents.xml",postData);
    QNetworkReply *rep=manager->post(*req,postData);
    connect(rep,SIGNAL(finished()),this,SLOT(slotGetWFEvents()));
}
void MainWindow::slotGetWFEvents() {
    QList<QString> eventsID;
    eventsID.clear();
    QByteArray data=((QNetworkReply*)sender())->readAll();
    //logger->File("getWFEvents+.xml",data);
    //ui->twEvents->clear();
    ui->twEvents->setRowCount(100);
    int row=0;
    QTableWidgetItem *item;
    QString eventType="",eventAlarmID="",eventAlarmText="",eventAlarmTypeText="",eventAlarmGroupText="";
    QDateTime eventDateTime;
    QXmlStreamReader *reader = new QXmlStreamReader(data);
    while(!reader->atEnd()) {
        token = reader->readNext();
        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="GetWFEventsResult") {
            do{
                token = reader->readNext();
                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="GetWFEventsResult")break;
                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="WFEvent") {
                    eventType="",eventAlarmID="",eventAlarmText="",eventAlarmTypeText="",eventAlarmGroupText="";

                    do{
                        token = reader->readNext();
                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="WFEvent")break;
                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Type") {
                            eventType=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                        }
                        else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Time") {
                            QString timeString=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                            eventDateTime=QDateTime::fromString(timeString,"yyyy-MM-ddTHH:mm:ss.z");
                            eventDateTime.setTimeSpec(Qt::UTC);
                        }
                        else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Alarm") {
                            do{
                                token = reader->readNext();
                                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Alarm")break;
                                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="ID") {
                                    eventAlarmID=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                                }
                                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Text") {
                                    do{
                                        token = reader->readNext();
                                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Text")break;
                                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Translation") {
                                            eventAlarmText=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                                        }
                                    }while(true);
                                }
                                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Type") {
                                    do{
                                        token = reader->readNext();
                                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Type")break;
                                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Text") {
                                            do{
                                                token = reader->readNext();
                                                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Text")break;
                                                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Translation") {
                                                    eventAlarmTypeText=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                                                }
                                            }while(true);
                                        }
                                    }while(true);
                                }
                                else if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Group") {
                                    do{
                                        token = reader->readNext();
                                        if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Group")break;
                                        if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Text") {
                                            do{
                                                token = reader->readNext();
                                                if(token==QXmlStreamReader::EndElement && reader->name().toString()=="Text")break;
                                                if(token==QXmlStreamReader::StartElement && reader->name().toString()=="Translation") {
                                                    eventAlarmGroupText=reader->readElementText(QXmlStreamReader::IncludeChildElements);
                                                }
                                            }while(true);
                                        }
                                    }while(true);
                                }
                            }while(true);
                        }
                    }while(true);
                    if(eventAlarmTypeText=="сообщение" || eventAlarmTypeText=="Message") continue;
                    if(eventType=="AlarmOff")
                        eventsID.append(eventAlarmID);
                    else {
                        if(!eventsID.contains(eventAlarmID)) {
                            item=ui->twEvents->item(row,0); if(item==NULL) {item=new QTableWidgetItem();ui->twEvents->setItem(row,0,item);} item->setText(eventType);
                            item=ui->twEvents->item(row,1); if(item==NULL) {item=new QTableWidgetItem();ui->twEvents->setItem(row,1,item);} item->setText(eventDateTime.toLocalTime().toString("dd-MM-yyyy HH:mm:ss"));
                            item=ui->twEvents->item(row,2); if(item==NULL) {item=new QTableWidgetItem();ui->twEvents->setItem(row,2,item);} item->setText(eventAlarmGroupText);
                            item=ui->twEvents->item(row,3); if(item==NULL) {item=new QTableWidgetItem();ui->twEvents->setItem(row,3,item);} item->setText(eventAlarmText);
                            row++;
                        }
                        else
                            eventsID.removeOne(eventAlarmID);
                    }

                    //qDebug()<<row<<" "<<eventType<<" "<<eventDateTime<<" "<<eventAlarmID<<" "<<eventAlarmText<<" "<<eventAlarmTypeText<<" "<<eventAlarmGroupText;
                }
            }while(true);
        }
    }
    ui->twEvents->resizeColumnsToContents();
    ui->twEvents->resizeRowsToContents();
    ui->twEvents->setRowCount(row);
    delete reader;
    delete sender();
}

void MainWindow::values(QString name, QString value) {
    //ui->txtPOSTRez->append(name+" = "+value);
    if(name=="ZAS Netz Leistungsschalter Rückmeldung") {
        if(HAZ_Sync!=value.toInt()) {
            HAZ_Sync=value.toInt();
            logger->Message(QString("ZAS Netz Leistungsschalter Rückmeldung: ")+(HAZ_Sync==-1?"Открыт":"Закрыт"));
            if(HAZ_Sync==0) {
                QFile file("tel.txt");
                file.open(QFile::ReadWrite);
                QTextStream stream(&file);
                while(!stream.atEnd()) {
                    QString tel=stream.readLine();
                    if(tel.length()>=11) {
                        logger->Message("SMS: "+tel+"Энергоблок "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+": Синхронизация отключена");
                        modem->sendSMS(tel,"Энергоблок "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+": Синхронизация отключена");
                    }
                }
                file.close();
                player->setMedia(QUrl("./sirena.mp3"));
                player->setVolume(100);
                player->setPosition(0);
                player->play();
                logger->Message("Включение сирены!!!");
            }
            else if(HAZ_Sync==-1) {
                QFile file("tel.txt");
                file.open(QFile::ReadWrite);
                QTextStream stream(&file);
                while(!stream.atEnd()) {
                    QString tel=stream.readLine();
                    if(tel.length()>=11) {
                        logger->Message("SMS: "+tel+"Энергоблок "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+": Синхронизация включена");
                        modem->sendSMS(tel,"Энергоблок "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+": Синхронизация включена");
                    }
                }
                file.close();
            }
            //    player->stop();
        }
        if(HAZ_Sync==-1 )
            ui->cbDisableSync->setChecked(false);
    }
    else if(name=="GEN1_HASanKunde Generator Leistungsschalter Rückmeldung") {
        if(GEN1_Sync!=value.toInt()) {
            GEN1_Sync=value.toInt();
            logger->Message(QString("GEN1_HASanKunde Generator Leistungsschalter Rückmeldung: ")+(GEN1_Sync==-1?"Открыт":"Закрыт"));
        }
    }
    else if(name=="GEN2_HASanKunde Generator Leistungsschalter Rückmeldung") {
        if(GEN2_Sync!=value.toInt()) {
            GEN2_Sync=value.toInt();
            logger->Message(QString("GEN2_HASanKunde Generator Leistungsschalter Rückmeldung: ")+(GEN2_Sync==-1?"Открыт":"Закрыт"));
        }
    }
    else if(name=="GEN3_HASanKunde Generator Leistungsschalter Rückmeldung") {
        if(GEN3_Sync!=value.toInt()) {
            GEN3_Sync=value.toInt();
            logger->Message(QString("GEN3_HASanKunde Generator Leistungsschalter Rückmeldung: ")+(GEN3_Sync==-1?"Открыт":"Закрыт"));
        }
    }
    else if(name=="GEN1_MFR3_PPU Generator Wirkleistung")
        GEN1_Pow=value.toDouble();
    else if(name=="GEN2_MFR3_PPU Generator Wirkleistung")
        GEN2_Pow=value.toDouble();
    else if(name=="GEN3_MFR3_PPU Generator Wirkleistung")
        GEN3_Pow=value.toDouble();
    else if(name=="GEN1_Messwerte T404 Raumluft") {
        GEN1_Temp=value.toDouble();
        ui->lblTemp1->setText(value);
    }
    else if(name=="GEN2_Messwerte T404 Raumluft") {
        GEN2_Temp=value.toDouble();
        ui->lblTemp2->setText(value);
    }
    else if(name=="GEN3_Messwerte T404 Raumluft") {
        GEN3_Temp=value.toDouble();
        ui->lblTemp3->setText(value);
    }
    else if(name=="GEN1_HASanKunde Läuft Rückmeldung")
        ui->cbG11->setChecked(value.toInt()!=-1);
    else if(name=="GEN2_HASanKunde Läuft Rückmeldung")
        ui->cbG21->setChecked(value.toInt()!=-1);
    else if(name=="GEN3_HASanKunde Läuft Rückmeldung")
        ui->cbG31->setChecked(value.toInt()!=-1);
    else if(name=="GEN1_HASanKunde Notstop Rückmeldung")
        ui->cbG12->setChecked(value.toInt()!=-1);
    else if(name=="GEN2_HASanKunde Notstop Rückmeldung")
        ui->cbG22->setChecked(value.toInt()!=-1);
    else if(name=="GEN3_HASanKunde Notstop Rückmeldung")
        ui->cbG32->setChecked(value.toInt()!=-1);
    else if(name=="GEN1_Logik Sammel-Störung") {
        ui->cbG13->setChecked(value.toInt()!=-1);
        bErrorE1=(value.toInt()!=-1);
    }
    else if(name=="GEN2_Logik Sammel-Störung") {
        ui->cbG23->setChecked(value.toInt()!=-1);
        bErrorE2=(value.toInt()!=-1);
    }
    else if(name=="GEN3_Logik Sammel-Störung") {
        ui->cbG33->setChecked(value.toInt()!=-1);
        bErrorE3=(value.toInt()!=-1);
    }
    if(bErrorW!=(bErrorW1||bErrorW2||bErrorW3))
        bErrorW=(bErrorW1||bErrorW2||bErrorW3);

    if(name=="GEN1_Logik Sammel-Warnung") {
        ui->cbG14->setChecked(value.toInt()!=-1);
        if(value.toInt()!=-1 && !bErrorW1) {
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
            logger->Message("Включение сирены W1!!!");
        }
        bErrorW1=(value.toInt()!=-1);
    }
    if(name=="GEN2_Logik Sammel-Warnung") {
        ui->cbG24->setChecked(value.toInt()!=-1);
        if(value.toInt()!=-1 && !bErrorW2) {
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
            logger->Message("Включение сирены W2!!!");
        }
        bErrorW2=(value.toInt()!=-1);
    }
    if(name=="GEN3_Logik Sammel-Warnung") {
        ui->cbG34->setChecked(value.toInt()!=-1);
        if(value.toInt()!=-1 && !bErrorW3) {
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
            logger->Message("Включение сирены W3!!!");
        }
        bErrorW3=(value.toInt()!=-1);
    }
    if(bErrorE!=(bErrorE1||bErrorE2||bErrorE3)) {
        bErrorE=(bErrorE1||bErrorE2||bErrorE3);
        if(bErrorE) {
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
            logger->Message("Включение сирены!!!");
        }
    }
    if(!bErrorE)
        ui->cbDisableErrors->setChecked(false);

    if(!bErrorW)
        ui->cbDisableWarnings->setChecked(false);
    if(player->state()==QMediaPlayer::PausedState || player->state()==QMediaPlayer::StoppedState) {
        if(HAZ_Sync==0 && !ui->cbDisableSync->isChecked()) {
            testingHorn=false;
            player->setMedia(QUrl("./sirena.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }
        else if(bErrorE && !ui->cbDisableErrors->isChecked()) {
            testingHorn=false;
            player->setMedia(QUrl("./Siren1.mp3"));
            player->setVolume(100);
            player->setPosition(0);
            player->play();
        }
        else if(bErrorW && !ui->cbDisableWarnings->isChecked()) {
            if(!warningTimer->isActive())
                warningTimer->start();
        }
    }
}

QByteArray MainWindow::DecodeBase65(QString strCoded) {
    QByteArray numArray;
    for (int index = 0; index < strCoded.length() / 2; index++)
        numArray[index] = (((int)strCoded[2 * index].toLatin1() - 65) * 16 + (int)strCoded[2 * index + 1].toLatin1() - 65);
    return numArray;
}

QByteArray MainWindow::EncodeXor(QByteArray bytes) {

    int num1 = 259;
    QByteArray numArray2;
    for (int index = 0; index < bytes.length(); index++) {
        int num2 = index % num1;
        if (num2 == 0)
            num2 = num1;
        numArray2[index] = (uint)bytes[index] ^ (uint)XORArray[num2 - 1];
    }
    for (int index = 0; index < bytes.length() / 2 - 1; index++)
    {
        if ((int)numArray2[index * 2] == 0 && (int)numArray2[index * 2 + 1] == 0)
        {
            numArray2[index * 2] = bytes[index * 2];
            numArray2[index * 2 + 1] = bytes[index * 2 + 1];
        }
    }
    return numArray2;
}
QString MainWindow::wfDecode(QString strPlain) {
    QByteArray bytes2 = EncodeXor(DecodeBase65(strPlain));
    return QString::fromUtf16((ushort*)bytes2.data(),bytes2.length()/2);
}


void MainWindow::on_cbDisableHorn_toggled(bool checked) {
    logger->Message(QString("Флаг отключения горна ")+(checked?"установлен":"снят"));
}

void MainWindow::on_btnTest_clicked() {
    player->setMedia(QUrl("./Alarm3.wav"));
    player->setVolume(100);
    player->setPosition(0);
    player->play();
    testingHorn=true;
    logger->Message(QString("Тестирование горна: ")+player->errorString());
}

void MainWindow::on_pbConnectModem_clicked()
{

    modem->connectModem(ui->cbPort->currentData().toString());
}
