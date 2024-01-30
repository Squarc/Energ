#ifndef NETWORKHOOKMANAGER_H
#define NETWORKHOOKMANAGER_H

#include <QNetworkAccessManager>
#include "networkreplyproxy.h"

class NetworkHookManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkHookManager(QObject *parent = 0);

    QNetworkReply *head(const QNetworkRequest &request);
    QNetworkReply *get(const QNetworkRequest &request);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);
signals:
    void Message(QString message);
    void pendingReplyProxy(NetworkReplyProxy *proxy);
public slots:

protected:
   QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKHOOKMANAGER_H
