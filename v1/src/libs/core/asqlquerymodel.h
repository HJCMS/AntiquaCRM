// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLQUERYMODEL_H
#define ANTIQUACRM_SQLQUERYMODEL_H

#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

class ASqlCore;

class ASqlQueryModel : public QSqlQueryModel {
  Q_OBJECT

protected:
  AntiquaCRM::ASqlCore *m_sql;
  const QString setHeaderTitel(const QString &text) const;
  const QString displayDate(const QVariant &value) const;
  const QString verticalHeader(int row, int role = Qt::DisplayRole) const;

public:
  explicit ASqlQueryModel(QObject *parent = nullptr);
  bool querySelect( const QString &sql);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLQUERYMODEL_H
