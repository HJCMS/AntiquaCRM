// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstablemodel.h"

#include <AntiquaCRM>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

ViewsTableModel::ViewsTableModel(const QSqlDatabase &db, QObject *parent)
    : QSqlTableModel{parent, db} {
  setObjectName("views_table_model");
  setEditStrategy(QSqlTableModel::OnRowChange);
}

QVariant ViewsTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  QSqlField field = record().field(section);
  return AntiquaCRM::AUtil::ucFirst(field.name());
}

const QString ViewsTableModel::getLabelText() {
  QString table = record().field(0).tableName();
  QString sql("SELECT obj_description(table_name::regclass)");
  sql.append(" FROM information_schema.tables WHERE ");
  sql.append("table_schema = current_schema AND ");
  sql.append("table_name='" + table + "' LIMIT 1;");
  QSqlQuery q = database().exec(sql);
  if (q.size() > 0) {
    q.next();
    return q.value(0).toString();
  }
  return table;
}
