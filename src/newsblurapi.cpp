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

NewsBlurApi::NewsBlurApi()
    : m_netMan(new NetworkManager(this))
{

}

void NewsBlurApi::login(QString username, QString password, NewsBlurResponse *out)
{
    QMap<QString, QString> params;
    params.insert("username", username);
    params.insert("password", password);
    m_netMan->apiPost("api/login", params, [this,out](QNetworkReply *reply) mutable {
        out->parseJSON(reply);
    });
}

NewsBlurResponse::NewsBlurResponse(QObject *parent)
{
}

bool NewsBlurResponse::responseOK()
{
    return m_result.value("result").toString() == QString("ok");
}

QVariantMap NewsBlurResponse::result()
{
    return m_result;
}

void NewsBlurResponse::parseJSON(QNetworkReply* response)
{
    QJsonDocument sd = QJsonDocument::fromJson(response->readAll());
    m_result = sd.toVariant().toMap();
    emit responseReceived();
    qDebug() << m_result;
}

