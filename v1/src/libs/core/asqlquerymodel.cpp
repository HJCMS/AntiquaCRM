// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlquerymodel.h"

#include "asqlcore.h"
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QSqlField>
#include <QSqlQuery>

namespace AntiquaCRM {

ASqlQueryModel::ASqlQueryModel(const QString &table, QObject *parent)
    : QSqlQueryModel{parent}, p_table(table) {
  m_sql = new AntiquaCRM::ASqlCore(this);
  p_record = QSqlRecord();
}

const QString ASqlQueryModel::setHeaderTitel(const QString &text) const {
  QString b(text);
  b.prepend(" ");
  b.append(" ");
  return b;
}

const QIcon ASqlQueryModel::setHeaderIcon(int column) const {
  Q_UNUSED(column);
  return QIcon(":icons/antiqua.png");
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

const QString ASqlQueryModel::fieldName(int column) {
  return fieldName(column);
}

bool ASqlQueryModel::querySelect(const QString &sql) {
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    p_record = q.record();
    setQuery(q);
    return true;
  } else if (!m_sql->lastError().isEmpty()) {
    QString erroMessage = m_sql->lastError().trimmed();
    qDebug() << Q_FUNC_INFO << erroMessage;
    emit sqlErrorMessage(p_table, erroMessage);
  }
  return false;
}

const QString ASqlQueryModel::tableName() const { return p_table; }

QVariant ASqlQueryModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  QVariant value = QSqlQueryModel::data(item, role);
  if (role != Qt::DisplayRole)
    return value;

  QVariant::Type _type = p_record.field(item.column()).type();
  if (_type == QVariant::DateTime) {
    return displayDate(value);
  }
  if (_type == QVariant::Bool) {
    return (value.toBool()) ? tr("Yes") : tr("No");
  }
  return value;
}

}; // namespace AntiquaCRM
