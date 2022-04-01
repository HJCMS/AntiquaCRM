/** @COPYRIGHT_HOLDER@ */

#ifndef SQLDATAMODEL_H
#define SQLDATAMODEL_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtSql/QSqlQueryModel>

class SQLDataModel : public QSqlQueryModel
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit SQLDataModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const;

  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // GENERALSETTINGSWIDGET_H
