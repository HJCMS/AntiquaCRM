// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlquerymodel.h"
#include "asqlcore.h"

#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QTime>

namespace AntiquaCRM
{

ASqlQueryModel::ASqlQueryModel(const QString& table, QObject* parent)
    : QSqlQueryModel{parent}, p_table(table) {
  m_sql = new AntiquaCRM::ASqlCore(this);

  connect(m_sql, SIGNAL(sendStatementError(const QSqlError&)),
          SIGNAL(sendSqlError(const QSqlError&)));
}

const QString ASqlQueryModel::setHeaderTitle(const QString& text) const {
  QString b(text);
  b.prepend(" ");
  b.append(" ");
  return b;
}

const QString ASqlQueryModel::displayDate(const QVariant& value) const {
  QDateTime dt(value.toDateTime());
  return QLocale::system().toString(dt, ANTIQUACRM_SHORT_DATE_DISPLAY);
}

const QString ASqlQueryModel::verticalHeader(int row, int role) const {
  Q_UNUSED(role);
  QString _s = QString::number(query().size());
  QString _r = QString::number(row + 1);
  return (_s.length() > 1) ? _r.rightJustified(_s.length(), ' ') : _r;
}

bool ASqlQueryModel::querySelect(const QString& sql) {
  if (!m_sql->status())
    return false;

  setQuery(sql, m_sql->db());
  if (lastError().isValid()) {
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << sql;
#endif
    emit sqlErrorMessage(p_table, lastError().text());
    return false;
  }
  return true;
}

const QString ASqlQueryModel::tableName() const {
  return p_table;
}

const QSqlRecord ASqlQueryModel::tableRecord() const {
  return m_sql->record(p_table);
}

const QSqlRecord ASqlQueryModel::queryRecord() const {
  return query().record();
}

const QString ASqlQueryModel::fieldName(int column) const {
  if (query().record().isEmpty() && p_table.isEmpty())
    return QString();

  if (query().record().isEmpty())
    return tableRecord().fieldName(column);

  return query().record().fieldName(column);
}

int ASqlQueryModel::column(const QString& fieldName) const {
  QSqlRecord _record = query().record();
  if (_record.isEmpty()) {
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "Empty Record";
#endif
    return -1;
  }

  for (int i = 0; i < _record.count(); i++) {
    if (_record.field(i).name() == fieldName)
      return i;
  }
  return -1;
}

QVariant ASqlQueryModel::data(const QModelIndex& item, int role) const {
  if (!item.isValid())
    return QVariant();

  QVariant value = QSqlQueryModel::data(item, role);
  if (role != Qt::DisplayRole)
    return value;

  QMetaType _type = queryRecord().field(item.column()).metaType();
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
  QString _time = QTime::currentTime().toString("HH:mm:ss");
  qint64 _limit = m_sql->getQueryLimit();
  qint64 _rows = query().size();
  QString _info;
  if (_rows >= _limit) {
    _info.append(tr("%1 - Result restricted to max '%2' Rows.").arg(_time, QString::number(_rows)));
  } else if (_rows > 0) {
    _info.append(tr("%1 - Query finished with '%2' Rows.").arg(_time, QString::number(_rows)));
  } else {
    _info.append(tr("%1 - Query without result!").arg(_time));
  }
  return _info;
}

}; // namespace AntiquaCRM
