#ifndef FILTEREDFEEDMODEL_H
#define FILTEREDFEEDMODEL_H

#include <QSortFilterProxyModel>

#include "feedmodel.h"

class FilteredFeedModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter)
public:
    explicit FilteredFeedModel(QObject *parent = 0);

    QString filter() const {
        return m_filter;
    }
    void setFilter(QString filter);

    void setSourceModel(QAbstractItemModel *sourceModel);

    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

    virtual bool canFetchMore(const QModelIndex& parent) const {
        return sourceModel() != NULL && sourceModel()->canFetchMore(parent);
    }
    virtual void fetchMore(const QModelIndex& parent) {
        sourceModel()->fetchMore(parent);
    }

private:
    QString m_filter;
};

#endif // FILTEREDFEEDMODEL_H
