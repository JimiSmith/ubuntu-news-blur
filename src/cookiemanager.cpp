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

#include <QDebug>
#include <QSettings>
#include <QDataStream>

#include "cookiemanager.h"

static const unsigned int JAR_VERSION = 23;
QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();
    quint32 version;
    stream >> version;
    if (version != JAR_VERSION)
        return stream;
    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i)
    {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            qWarning() << "CookieManager: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}
QT_END_NAMESPACE

CookieManager::CookieManager(QObject *parent)
    : QNetworkCookieJar(parent)
{
    load();
}

bool CookieManager::insertCookie(const QNetworkCookie & cookie)
{
    bool result = QNetworkCookieJar::insertCookie(cookie);
    save();
    return result;
}

void CookieManager::load()
{
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    QSettings settings("mrsmith", "newsblur");
    settings.beginGroup("auth");
    setAllCookies(qvariant_cast<QList<QNetworkCookie> >(settings.value("cookies")));
    settings.endGroup();
}

void CookieManager::save()
{
    QList<QNetworkCookie> cookies = allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if (cookies.at(i).isSessionCookie())
            cookies.removeAt(i);
    }
    QSettings settings("mrsmith", "newsblur");
    settings.beginGroup("auth");
    settings.setValue("cookies", QVariant::fromValue<QList<QNetworkCookie> >(cookies));
    settings.endGroup();
}

bool CookieManager::hasCookies()
{
    return allCookies().length() > 0;
}

