// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlquerymodel.h"

#include <QDateTime>
#include <QLocale>
#include <QSqlField>
#include <QSqlQuery>

#include "asqlcore.h"

namespace AntiquaCRM {

ASqlQueryModel::ASqlQueryModel(QObject *parent) : QSqlQueryModel{parent} {
  m_sql = new AntiquaCRM::ASqlCore(this);
}

const QString ASqlQueryModel::setHeaderTitel(const QString &text) const {
  QString b(text);
  b.prepend(" ");
  b.append(" ");
  return b;
}

const QString ASqlQueryModel::displayDate(const QVariant &value) const {
  QDateTime dt(value.toDateTime());
  return QLocale::system().toString(dt, "dd MMMM yyyy");
}

const QString ASqlQueryModel::verticalHeader(int row, int role) const {
  Q_UNUSED(role);
  QString s = QString::number(query().size());
  QString r = QString::number(row + 1);
  return (s.length() > 1) ? r.rightJustified(s.length(), ' ') : r;
}

bool ASqlQueryModel::querySelect(const QString &sql) {
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    setQuery(q);
    return true;
  } else if (!m_sql->lastError().isEmpty()) {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
  return false;
}

}; // namespace AntiquaCRM
