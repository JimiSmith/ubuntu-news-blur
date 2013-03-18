#include <QDebug>
#include "apihelper.h"
#include "cookiemanager.h"
#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    m_netMan(new QNetworkAccessManager(this)),
    m_cookieJar(new CookieManager(this))
{
    m_netMan->setCookieJar(m_cookieJar);
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void NetworkManager::apiPost(QString url, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(post(ApiHelper::apiPostRequest(url), ApiHelper::getParamString(params).toLatin1()), complete);
}

void NetworkManager::apiGet(QString url, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(get(ApiHelper::apiGetRequest(url, params)), complete);
}

QNetworkReply* NetworkManager::post(QNetworkRequest request, QByteArray body)
{
    qDebug() << "POSTing" << request.url();
    return m_netMan->post(request, body);
}

QNetworkReply* NetworkManager::get(QNetworkRequest request)
{
    qDebug() << "GETing" << request.url();
    return m_netMan->get(request);
}

void NetworkManager::replyFinished(QNetworkReply *reply)
{
    qDebug() << "Got reply" << reply->url() << reply->error();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error with request" << reply->url() << reply->error();
        auto func = m_operations.value(reply);
        if (func) {
            func(NULL);
            m_operations.remove(reply);
        } else {
            emit requestComplete(NULL);
        }
    } else {
        QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if(!possibleRedirectUrl.isEmpty()) { //we're being redirected
            m_netMan->get(QNetworkRequest(possibleRedirectUrl));
        } else { //we're at the endpoint
            auto func = m_operations.value(reply);
            if (func) {
                func(reply);
                m_operations.remove(reply);
            } else {
                emit requestComplete(reply);
            }
        }
    }
    reply->deleteLater();
}

bool NetworkManager::loggedIn()
{
    return m_cookieJar->hasCookies();
}
