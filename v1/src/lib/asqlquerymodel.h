// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLQUERYMODEL_H
#define ANTIQUACRM_SQLQUERYMODEL_H

#include <AGlobal>
#include <QIcon>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

class ASqlCore;

/**
 * @brief SqlQueryModel for TableViews
 * @section sql
 */
class ANTIQUACRM_LIBRARAY ASqlQueryModel : public QSqlQueryModel {
  Q_OBJECT

private:
  const QString p_table;
  QSqlRecord p_record;

protected:
  AntiquaCRM::ASqlCore *m_sql;
  const QString setHeaderTitle(const QString &text) const;
  const QIcon setHeaderIcon(int column) const;
  const QString displayDate(const QVariant &value) const;
  const QString verticalHeader(int row, int role = Qt::DisplayRole) const;

Q_SIGNALS:
  void sqlErrorMessage(const QString &table, const QString &message);

public:
  explicit ASqlQueryModel(const QString &table, QObject *parent = nullptr);
  virtual const QMap<int, QString> headerList() const = 0;
  bool querySelect(const QString &sql);
  const QString tableName() const;
  const QString fieldName(int column = 0) const;
  const int column(const QString &fieldName) const;
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLQUERYMODEL_H
