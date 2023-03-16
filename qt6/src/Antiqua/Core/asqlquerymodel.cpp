// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlquerymodel.h"
#include "asqlcore.h"

#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QSqlField>
#include <QSqlQuery>
#include <QTime>

namespace AntiquaCRM {

ASqlQueryModel::ASqlQueryModel(const QString &table, QObject *parent)
    : QSqlQueryModel{parent}, p_table(table) {
  m_sql = new AntiquaCRM::ASqlCore(this);
  p_queryRecord = QSqlRecord();
  p_queryResult = 0;
}

const QString ASqlQueryModel::setHeaderTitle(const QString &text) const {
  QString b(text);
  b.prepend(" ");
  b.append(" ");
  return b;
}

const QString ASqlQueryModel::displayDate(const QVariant &value) const {
  QDateTime dt(value.toDateTime());
  return QLocale::system().toString(dt, ANTIQUACRM_SHORT_DATE_DISPLAY);
}

const QString ASqlQueryModel::verticalHeader(int row, int role) const {
  Q_UNUSED(role);
  QString s = QString::number(query().size());
  QString r = QString::number(row + 1);
  return (s.length() > 1) ? r.rightJustified(s.length(), ' ') : r;
}

bool ASqlQueryModel::querySelect(const QString &sql) {
  p_queryResult = 0;
  QSqlQuery _query = m_sql->query(sql);
  // if no errors clear old table content
  if (m_sql->lastError().isEmpty())
    clear();

  if (_query.size() > 0) {
    p_queryResult = _query.size();
    p_queryRecord = _query.record();
    setQuery(sql);
    return true;
  } else if (!m_sql->lastError().isEmpty()) {
    QString erroMessage = m_sql->lastError().trimmed();
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << erroMessage;
#endif
    emit sqlErrorMessage(p_table, erroMessage);
    return false;
  }
  // no errors!
  return true;
}

const QString ASqlQueryModel::tableName() const { return p_table; }

const QSqlRecord ASqlQueryModel::tableRecord() const {
  return m_sql->record(p_table);
}

const QSqlRecord ASqlQueryModel::queryRecord() const {
  if (p_queryRecord.isEmpty())
    return QSqlRecord();

  return p_queryRecord;
}

const QString ASqlQueryModel::fieldName(int column) const {
  if (p_queryRecord.isEmpty())
    return QString();

  return p_queryRecord.fieldName(column);
}

int ASqlQueryModel::column(const QString &fieldName) const {
  if (p_queryRecord.isEmpty())
    return -1;

  for (int i = 0; i < p_queryRecord.count(); i++) {
    if (p_queryRecord.field(i).name() == fieldName)
      return i;
  }

  return -1;
}

QVariant ASqlQueryModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  QVariant value = QSqlQueryModel::data(item, role);
  if (role != Qt::DisplayRole)
    return value;

  QMetaType _type = p_queryRecord.field(item.column()).metaType();
  // QString _name = p_record.field(item.column()).name();
  // qDebug() << _name << _type << value;
  if (_type.id() == QMetaType::QDateTime) {
    return displayDate(value);
  }

  if (_type.id() == QMetaType::Bool) {
    return (value.toBool()) ? tr("Yes") : tr("No");
  }

  if (_type.id() == QMetaType::LongLong || _type.id() == QMetaType::ULongLong) {
    return (value.toString().length() > 5) ? value : QVariant();
  }
  return value;
}

const QString ASqlQueryModel::queryResultInfo() {
  QString time = QTime::currentTime().toString("HH:mm:ss");
  QString info;
  if (p_queryResult > 0) {
    info.append(tr("%1 - Query finished with '%2' Rows.")
                    .arg(time, QString::number(p_queryResult)));
  } else {
    info.append(tr("%1 - Query without result!").arg(time));
  }
  return info;
}

}; // namespace AntiquaCRM
