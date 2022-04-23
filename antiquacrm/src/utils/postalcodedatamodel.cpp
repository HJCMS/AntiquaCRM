// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodedatamodel.h"
// Privat
#include <SqlCore>
// Qt
#include <QtCore>
#include <QtGui>
#include <QtSql>

PostalCodeDataModel::PostalCodeDataModel(QObject *parent)
    : QSqlQueryModel{parent} {
  setObjectName("PostalCodeDataModel");
  m_sql = new HJCMS::SqlCore(this);
}

void PostalCodeDataModel::fetchTableData() {
  QString sql("SELECT p_plz FROM ");
  sql.append("ref_postalcodes_de");
  sql.append(" WHERE p_plz>1000 ORDER BY p_plz;");

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
      int i = 0;
    while (q.next()) {
      QString v = q.value(0).toString();
      if (!v.isEmpty())
      {
        QModelIndex index;
        setData(index,v,Qt::DisplayRole);
      }
    }
  }
}

QVariant PostalCodeDataModel::data(const QModelIndex &index, int role) {
  return QVariant();
}

int PostalCodeDataModel::rowCount(const QModelIndex &parent) { return 0; }

bool PostalCodeDataModel::setData(const QModelIndex &index,
                                  const QVariant &value, int role) {
  return false;
}
