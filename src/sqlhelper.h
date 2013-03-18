#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QStringList>
#include <QVariantMap>
#include <QVariantList>
#include <QString>

class SqlHelper
{
public:
    static void createTableIfNeeded(QString tableName, QStringList values);
    static bool addOrUpdateFeedBatch(QVariantList subData);
    static bool addOrUpdateFeed(QVariantMap subData);
    static bool setAllFeedsToLoadingState();
};

#endif // SQLHELPER_H
