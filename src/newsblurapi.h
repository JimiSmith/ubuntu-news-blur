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

#ifndef NewsBlurApi_H
#define NewsBlurApi_H

#include <QObject>
#include <QVariantMap>
#include <QtConcurrent>
#include <functional>

class NetworkManager;
class QNetworkReply;

class NewsBlurResponse : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ok READ responseOK)
    Q_PROPERTY(bool authenticated READ authenticated)
public:
    explicit NewsBlurResponse(QObject *parent = 0);

    void parseJSON(QNetworkReply *response, std::function<void(QVariantMap result)> complete);
    bool responseOK();
    bool authenticated();
    QVariantMap result();

signals:
    void responseReceived();

private:
    void parseJSONInternal(QByteArray response, std::function<void(QVariantMap result)> complete);
    QFutureWatcher<void> m_watcher;
    QVariantMap m_result;
    bool m_error;
};

class NewsBlurApi : public QObject
{
    Q_OBJECT
public:
    explicit NewsBlurApi();
    Q_INVOKABLE bool isLoggedIn();
    Q_INVOKABLE NewsBlurResponse* login(QString username, QString password);
    Q_INVOKABLE NewsBlurResponse* getFeeds();
    Q_INVOKABLE NewsBlurResponse* updateUnreadCount();

private:
    NetworkManager *m_netMan;

};

#endif // NewsBlurApi_H
