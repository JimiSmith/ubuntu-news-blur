/*
    A News Blur client for Ubuntu Phone
    Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "newsblurapi.h"

#include <QVariant>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QtCore/QDebug>
#include <QNetworkCookie>
#include "networkmanager.h"
#include "sqlhelper.h"

NewsBlurApi::NewsBlurApi()
    : m_netMan(new NetworkManager(this))
{

}

bool NewsBlurApi::isLoggedIn()
{
    return m_netMan->loggedIn();
}

void NewsBlurApi::login(QString username, QString password, NewsBlurResponse *out)
{
    QMap<QString, QString> params;
    params.insert("username", username);
    params.insert("password", password);
    m_netMan->apiPost("api/login", params, [this,out](QNetworkReply *reply) {
        out->parseJSON(reply, [](QVariantMap result) {});
    });
}

void NewsBlurApi::getFeeds(NewsBlurResponse *out)
{
    m_netMan->apiGet("reader/feeds", QMap<QString, QString>(), [this,out](QNetworkReply* reply) {
        out->parseJSON(reply, [](QVariantMap result) {
            QVariantList feedList = result.value("feeds").toMap().values();
            SqlHelper::addOrUpdateFeedBatch(feedList);
        });
    });
}

void NewsBlurApi::updateUnreadCount(NewsBlurResponse *out)
{
    m_netMan->apiGet("reader/refresh_feeds", QMap<QString, QString>(), [this,out](QNetworkReply* reply) {
        out->parseJSON(reply, [](QVariantMap result) {
            QVariantList feedList = result.value("feeds").toMap().values();
            SqlHelper::addOrUpdateFeedCountsBatch(feedList);
        });
    });
}

NewsBlurResponse::NewsBlurResponse(QObject *parent)
    : QObject(parent),
    m_error(false)
{
    connect(&m_watcher, SIGNAL(finished()), this, SIGNAL(responseReceived()));
}

bool NewsBlurResponse::responseOK()
{
    return !m_error && m_result.value("result").toString() == QString("ok");
}

bool NewsBlurResponse::authenticated()
{
    return m_result.value("authenticated").toInt() == 1;
}

QVariantMap NewsBlurResponse::result()
{
    return m_result;
}

void NewsBlurResponse::parseJSON(QNetworkReply* response, std::function<void(QVariantMap result)> complete)
{
    if (response == NULL) {
        m_error = true;
        emit responseReceived();
    } else {
        m_error = false;
        QFuture<void> respFuture = QtConcurrent::run(this, &NewsBlurResponse::parseJSONInternal, response->readAll(), complete);
        m_watcher.setFuture(respFuture);
    }
}

void NewsBlurResponse::parseJSONInternal(QByteArray response, std::function<void(QVariantMap result)> complete)
{
    QJsonDocument sd = QJsonDocument::fromJson(response);
    m_result = sd.toVariant().toMap();
    complete(m_result);
}

