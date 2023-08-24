// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqldataquery.h"

#include <QSqlDatabase>
#include <QVariant>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

namespace AntiquaCRM {

static const QSqlRecord asql_table_record(const QString &name) {
  if (name.isEmpty()) {
    qWarning("AntiquaCRM::ASqlDataQuery 'Missing SQL table name'!");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "QString::isEmpty" << name.isEmpty();
#endif
    return QSqlRecord();
  }
  QSqlDatabase db = QSqlDatabase::database(ANTIQUACRM_CONNECTION_DOMAIN);
  // #ifdef ANTIQUA_DEVELOPEMENT
  //   QSqlRecord _r = db.record(name);
  //   for(int i = 0; i < _r.count(); i++) {
  //     QSqlField _f = _r.field(i);
  //     qDebug() << "F:" << _f.name() << _f.metaType().id() <<
  //     _f.metaType().name();
  //   }
  // #endif
  return (db.isValid() ? db.record(name) : QSqlRecord());
}

ASqlDataQuery::ASqlDataQuery(const QString &tableName)
    : p_record{asql_table_record(tableName)} {}

ASqlDataQuery::ASqlDataQuery(const QSqlRecord &record)
    : p_record{record}, p_data{} {
  if (p_record.isEmpty()) {
    qWarning("AntiquaCRM::ASqlDataQuery Invalid class Initialisation!");
  }
}

ASqlDataQuery::ASqlDataQuery(const ASqlDataQuery &other)
    : p_record{other.p_record}, p_data{other.p_data} {
  if (p_record.isEmpty()) {
    qWarning("AntiquaCRM::ASqlDataQuery Invalid class Initialisation!");
  }
}

const QSqlRecord ASqlDataQuery::record() const { return p_record; }

int ASqlDataQuery::size() const { return p_data.size(); }

const QString ASqlDataQuery::tableName() const {
  if (isValid()) {
    return p_record.field(0).tableName();
  }
  return QString();
}

bool ASqlDataQuery::isValid() const {
  return (p_record.isEmpty() ? false : true);
}

const QStringList ASqlDataQuery::columnNames() const {
  QStringList fields;
  if (!isValid())
    return fields;

  for (int i = 0; i < p_record.count(); i++) {
    fields << p_record.fieldName(i);
  }

  return fields;
}

const QSqlField ASqlDataQuery::getProperties(const QString &column) const {
  QSqlField _field;
  if (!isValid())
    return _field;

  for (int i = 0; i < p_record.count(); i++) {
    if (p_record.fieldName(i) == column) {
      _field = p_record.field(i);
      break;
    }
  }
  return _field;
}

const QMetaType ASqlDataQuery::getType(const QString &column) const {
  QSqlField field = getProperties(column);
  if (field.isValid())
    return field.metaType();

  return QMetaType();
}

void ASqlDataQuery::setValue(const QString &column, const QVariant &value) {
  if (!isValid())
    return;

  const QSqlField _field = getProperties(column);
  if (!_field.isValid())
    return;

  const QMetaType _type = _field.metaType();
  if (_type != value.metaType()) {
    qWarning("Warning MetaType for '%s' require '%s' but get '%s'!",
             qPrintable(column), _type.name(), value.metaType().name());
  }

  if (_field.requiredStatus() == QSqlField::Required && value.isNull()) {
    if (_field.defaultValue().isNull()) {
      qFatal("Invalid value! Field:'%s' is required and can't null.",
             qPrintable(column));
      return;
    }
    p_data.insert(column, _field.defaultValue());
    return;
  }

  if (_type.id() == QMetaType::QString && _field.length() > 0) {
    if (value.toString().length() > _field.length()) {
      qFatal("Invalid datasize! '%s' max length is '%d'", qPrintable(column),
             _field.length());
      return;
    }
  }

  if (value.isNull() && p_data.contains(column)) {
    p_data.remove(column);
    return;
  }

  p_data.insert(column, value);
}

const QVariant ASqlDataQuery::getValue(const QString &column) {
  if (!isValid())
    return QVariant();

  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == column)
      return it.value();
  }
  return QVariant();
}

const QHash<QString, QVariant> ASqlDataQuery::getDataset() { return p_data; }

}; // namespace AntiquaCRM
