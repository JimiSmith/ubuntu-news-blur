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

class NetworkManager;
class QNetworkReply;

class NewsBlurResponse : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ok READ responseOK)
    Q_PROPERTY(QVariantMap result READ result)
public:
    explicit NewsBlurResponse(QObject *parent = 0);

    void parseJSON(QNetworkReply *response);
    bool responseOK();
    QVariantMap result();

signals:
    void responseReceived();

private:
    QVariantMap m_result;
};

class NewsBlurApi : public QObject
{
    Q_OBJECT
public:
    explicit NewsBlurApi();
    Q_INVOKABLE void login(QString username, QString password, NewsBlurResponse *out);

private:
    NetworkManager *m_netMan;

};

#endif // NewsBlurApi_H
