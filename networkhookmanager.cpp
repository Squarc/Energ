#include "networkhookmanager.h"
#include "networkreplyproxy.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkRequest>

NetworkHookManager::NetworkHookManager(QObject *parent):QNetworkAccessManager(parent)
{
}

QNetworkReply* NetworkHookManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString method;
    QNetworkReply *real = QNetworkAccessManager::createRequest(op, request, outgoingData);
    QList<QByteArray> list = request.rawHeaderList();
    QString header;
    foreach(QByteArray ba, list) {
        if(header.length()>0)header+="<br>";
        header+=ba;
        header+="="+request.rawHeader(ba);
    }
    header+="<br>";
    switch (op) {
    case 1:
        method="<b>HEAD</b>";
        break;
    case 2:
        method="<b>GET</b>";
        break;
    case 3:
        method="<b>PUT</b>";
        break;
    case 4:
        method="<b>POST</b>";
        break;
    case 5:
        method="<b>DELETE</b>";
        break;
    case 6:
        method="<b>CUSTOM</b>";
        break;
    default:
        method="<b></b>";
        break;
    }
    emit Message(method+" "+request.url().toString());
    emit Message(header);
    NetworkReplyProxy *proxy = new NetworkReplyProxy(this,real);
    emit pendingReplyProxy(proxy);
    return proxy;
}

QNetworkReply *NetworkHookManager::head(const QNetworkRequest &request)
{
    return QNetworkAccessManager::head(request);
}

QNetworkReply *NetworkHookManager::get(const QNetworkRequest &request)
{
    return QNetworkAccessManager::get(request);
}

QNetworkReply *NetworkHookManager::post(const QNetworkRequest &request, const QByteArray &data)
{
    return QNetworkAccessManager::post(request,data);
}

QNetworkReply *NetworkHookManager::put(const QNetworkRequest &request, const QByteArray &data)
{
    return QNetworkAccessManager::put(request,data);
}
