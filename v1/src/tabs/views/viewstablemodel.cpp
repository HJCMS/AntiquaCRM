// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstablemodel.h"

#include <QDebug>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

ViewsTableModel::ViewsTableModel(const QSqlDatabase &db, QObject *parent)
    : QSqlTableModel{parent, db} {
  setObjectName("views_table_model");
  setEditStrategy(QSqlTableModel::OnRowChange);
}

const QString ViewsTableModel::ucFirst(const QString &title) const {
  QStringList array = title.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

QVariant ViewsTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  QSqlField field = record().field(section);
  return ucFirst(field.name());
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
