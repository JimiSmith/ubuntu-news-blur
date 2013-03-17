#include <QDebug>
#include <QSettings>
#include <QDateTime>

#include "apihelper.h"

QString ApiHelper::appendParamsToUrl(QString url, QMap<QString, QString> queryParams)
{
    QString newUrl = QString(url);
    if (queryParams.size() > 0) {
        newUrl.append(QString("?%0").arg(getParamString(queryParams)));
    }
    return newUrl;
}

QNetworkRequest ApiHelper::apiGetRequest(QString endpoint, QMap<QString, QString> queryParams)
{
    QString url = appendParamsToUrl(QString("https://newsblur.com/%0").arg(endpoint), queryParams);
    QNetworkRequest r(url);
    return r;
}

QNetworkRequest ApiHelper::apiPostRequest(QString endpoint)
{
    QString url = QString("https://newsblur.com/%0").arg(endpoint);
    QNetworkRequest r(url);
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return r;
}

QString ApiHelper::getParamString(QMap<QString, QString> queryParams)
{
    QString newUrl = "";
    if (queryParams.size() > 0) {
        foreach (QString key, queryParams.keys()) {
            QString value = queryParams.value(key);
            newUrl.append(QString("%0=%1&").arg(key).arg(QString(QUrl::toPercentEncoding(value))));
        }
        newUrl.chop(1);
    }
    return newUrl;
}
