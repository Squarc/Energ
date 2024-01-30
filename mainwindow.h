#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QTextCodec>
#include <QNetworkReply>
#include <QFile>
#include <QTextEdit>
#include <QDateTime>
#include <QtXml>
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTcpServer>
#include <QCheckBox>
#include <QMenu>
#include "GSMModem.h"

#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct LanguageRes {
        int LanguageID;
        QString Description;
        int Active,Used;
    };
    GSMModem *modem;
    int secondCounter;
    QTimer *warningTimer;
    QList<LanguageRes*> languages;
    bool testingHorn;
    QTcpServer *server;
    QMediaPlayer *player;
    Logger *logger;
    //enum NetworkPack {packRegSignals,packGetParameters,packConnect,packReadLanguages};
    //NetworkPack networkpack;
    QNetworkAccessManager *manager;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    bool event(QEvent *event);

    bool key_control, key_shift, key_alt;

    QNetworkRequest *req;
    //QNetworkReply *rep;
    QString wfDecode(QString strPlain);
    QByteArray EncodeXor(QByteArray bytes);
    QByteArray DecodeBase65(QString strCoded);
    QAudioDeviceInfo info;

    //Phonon::MediaObject *mediaObject;
    //Phonon::AudioOutput *audioOutput;
    //Phonon::MediaObject *metaInformationResolver;

    QByteArray getUsersData,getAlarmTypesData,getAlarmGroupsData;
    //QByteArray getSessionGUID;
    QByteArray getRegisterSignals;
    QByteArray getParameters;
    QByteArray sessionGUID;
    QByteArray XORArray;
    void registerSignals();
    void getSessionGUID();
    void readLanguages();
    void getUsers();
    void getAlarmTypes();
    void getAlarmGroups();
    void getWFEvents();
    void parameters();
    void values(QString name,QString value);

    int HAZ_Sync,GEN1_Sync,GEN2_Sync,GEN3_Sync;
    bool bErrorE,bErrorE1,bErrorE2,bErrorE3,bErrorW,bErrorW1,bErrorW2,bErrorW3;
    double GEN1_Pow,GEN2_Pow,GEN3_Pow,GEN1_Temp,GEN2_Temp,GEN3_Temp;

    QStringList list;
    QString chars;
    QXmlStreamReader::TokenType token;
    QString base65;
    //void on_readLanguages_finished(QNetworkReply *reply);

public slots:
    void timer_elapsed();
    void warningTimer_elapsed();
    //void manager_finished(QNetworkReply* reply);
    void playerStateChanged(QMediaPlayer::State s);
    void newConnection();
private slots:
    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void setTrayIconActions();
    void showTrayIcon();

    void on_cbDisableHorn_toggled(bool checked);

    void on_btnTest_clicked();
    void slotReadLanguages();
    void slotGetUsers();
    void slotGetAlarmTypes();
    void slotGetAlarmGroups();
    void slotGetWFEvents();
    void registerSignals_finished();
    void sessionGUID_finished();
    void getParameters_finished();

    void on_pbConnectModem_clicked();
    void operatorName(QString name);
    void signalPower(int value);
    void portState(bool state);
    void slotMessage(QString message);
private:
    Ui::MainWindow *ui;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
