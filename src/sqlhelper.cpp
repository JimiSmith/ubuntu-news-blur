#include "sqlhelper.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QDebug>

void SqlHelper::createTableIfNeeded(QString tableName, QStringList values)
{
    QSqlQuery tableQuery;
    QString sqlQuery = QString("SELECT count(name) FROM sqlite_master WHERE type = 'table' AND name = '%1'").arg(tableName);
    bool tableRet = tableQuery.exec(sqlQuery);

    if (!tableRet) {
        qWarning() << "There was an error creating the DB:" << tableQuery.lastError();
    }

    if (tableQuery.first() && tableQuery.value(0).toInt() > 0) {
        return;
    }

    QSqlQuery createQuery;
    QString sqlCreateQuery = QString("CREATE TABLE %1 (").arg(tableName);

    foreach (QString v, values) {
        sqlCreateQuery.append(v).append(",");
    }

    sqlCreateQuery.chop(1); //remove the final comma
    sqlCreateQuery.append(")");

    bool ret = createQuery.exec(sqlCreateQuery);

    if (!ret) {
        qWarning() << "There was an error creating the DB:" << createQuery.lastError();
    }
}

bool SqlHelper::addOrUpdateFeedBatch(QVariantList subData)
{
    bool result = true;
    int count = subData.length();
    for (int i = 0; i < count; ++i) {
        QVariant m = subData.at(i);
        result = addOrUpdateFeed(m.toMap()) && result;
    }

    return result;
}

bool SqlHelper::addOrUpdateFeed(QVariantMap subData)
{
    QSqlDatabase::database().transaction();

    QString id = QString::number(subData.value("id").toInt());

    QSqlQuery existsQuery;
    existsQuery.prepare("SELECT count(feed_id) FROM feeds WHERE feed_id=:id");
    existsQuery.bindValue(":id", id);
    bool existsRet = existsQuery.exec();

    if (!existsRet) {
        qWarning() << "Error with existsQuery:" << existsQuery.lastError();
        QSqlDatabase::database().rollback();
        return false;
    }

    bool exists = existsQuery.next() && existsQuery.value(0).toInt() > 0;

    QSqlQuery dataQuery;

    if (exists) {
        dataQuery.prepare("UPDATE feeds SET title=:title, needs_update=1, url=:url, seconds_since_update=:seconds_since_update WHERE feed_id=:id");
    } else {
        dataQuery.prepare("INSERT INTO feeds (feed_id, title, url, unread, unread_focus, needs_update, seconds_since_update)"
                " VALUES (:id, :title, :url, 0, 0, 1, :seconds_since_update)");
    }

    dataQuery.bindValue(":id", id);
    dataQuery.bindValue(":title", subData.value("feed_title").toString());
    dataQuery.bindValue(":seconds_since_update", subData.value("updated_seconds_ago").toString());
    dataQuery.bindValue(":url", subData.value("feed_link").toString());

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        qWarning() << "Error with dataQuery:" << dataQuery.lastError();
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();

    return true;
}

bool SqlHelper::addOrUpdateFeedCountsBatch(QVariantList subData)
{
    bool result = true;
    int count = subData.length();
    for (int i = 0; i < count; ++i) {
        QVariant m = subData.at(i);
        result = addOrUpdateFeedCount(m.toMap()) && result;
    }

    return result;
}

bool SqlHelper::addOrUpdateFeedCount(QVariantMap subData)
{
    QSqlDatabase::database().transaction();
    QSqlQuery dataQuery;

    dataQuery.prepare("UPDATE feeds SET needs_update=0, unread=:unread, unread_focus=:unread_focus WHERE feed_id=:id");

    QString id = QString::number(subData.value("id").toInt());
    dataQuery.bindValue(":id", id);
    dataQuery.bindValue(":unread", subData.value("nt"));
    dataQuery.bindValue(":unread_focus", subData.value("ng"));

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        qWarning() << "Error with dataQuery:" << dataQuery.lastError();
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();

    return true;
}

bool SqlHelper::setAllFeedsToLoadingState()
{
    QSqlDatabase::database().transaction();
    QSqlQuery dataQuery;

    dataQuery.prepare("UPDATE subscriptions SET needs_update=1");

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();

    return true;
}

