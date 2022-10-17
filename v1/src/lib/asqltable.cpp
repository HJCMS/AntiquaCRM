// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqltable.h"

#include <QSqlDatabase>
#include <QVariant>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

namespace AntiquaCRM {

static const QSqlRecord asql_table_record(const QString &name) {
  if (name.isEmpty()) {
    qWarning("AntiquaCRM::ASqlTable 'Missing SQL table name'!");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "QString::isEmpty" << name.isEmpty();
#endif
    return QSqlRecord();
  }
  QSqlDatabase db = QSqlDatabase::database(ANTIQUACRM_CONNECTION_DOMAIN);
  return (db.isValid() ? db.record(name) : QSqlRecord());
}

ASqlTable::ASqlTable(const QString &tableName)
    : tableRecord{asql_table_record(tableName)} {}

ASqlTable::ASqlTable(const ASqlTable &other)
    : tableRecord{other.tableRecord}, p_data{other.p_data} {
  if (!isValid()) {
    qWarning("AntiquaCRM::ASqlTable Invalid class Initialisation!");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "isValid" << isValid();
#endif
  }
}

ASqlTable::ASqlTable(const QSqlRecord &record) : tableRecord{record}, p_data{} {
  if (!isValid()) {
    qWarning("AntiquaCRM::ASqlTable Invalid class Initialisation!");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "isValid" << isValid();
#endif
  }
}

const QSqlRecord ASqlTable::record() const { return tableRecord; }

const QString ASqlTable::tableName() const {
  if (isValid()) {
    return tableRecord.field(0).tableName();
  }
  return QString();
}

bool ASqlTable::isValid() const {
  return (tableRecord.isEmpty() ? false : true);
}

const QStringList ASqlTable::columnNames() const {
  QStringList fields;
  if (!isValid())
    return fields;

  for (int i = 0; i < tableRecord.count(); i++) {
    fields << tableRecord.fieldName(i);
  }

  return fields;
}

const QSqlField ASqlTable::getProperties(const QString &column) const {
  QSqlField field;
  if (!isValid())
    return field;

  for (int i = 0; i < tableRecord.count(); i++) {
    if (tableRecord.fieldName(i) == column) {
      field = tableRecord.field(i);
      break;
    }
  }
  return field;
}

const QMetaType ASqlTable::getType(const QString &column) const {
  QSqlField field = getProperties(column);
  if (field.isValid()) {
    QVariant::Type _t = field.type();
    return QMetaType(QMetaType::type(QVariant::typeToName(_t)));
  }
  return QMetaType();
}

void ASqlTable::setValue(const QString &column, const QVariant &value) {
  if (!isValid())
    return;

  QSqlField field = getProperties(column);
  if (!field.isValid())
    return;

  if (field.type() != value.type()) {
    int metaCheck;
    QVariant from(field.defaultValue());
    if (!QMetaType::compare(&from, &value, getType(column).id(), &metaCheck)) {
      qWarning("Warning MetaType for '%s' require '%s' but get '%s'!",
               qPrintable(column), QVariant(field.type()).typeName(),
               QVariant(value.type()).typeName());
    }
  }

  if (field.requiredStatus() == QSqlField::Required && value.isNull()) {
    if (field.defaultValue().isNull()) {
      qFatal("Invalid value! Field:'%s' is required and can't null.",
             qPrintable(column));
      return;
    }
    p_data.insert(field.name(), field.defaultValue());
    return;
  }

  if (field.type() == QVariant::String && field.length() > 0) {
    if (value.toString().length() > field.length()) {
      qFatal("Invalid datasize! '%s' max length is '%d'", qPrintable(column),
             field.length());
      return;
    }
  }

  if (value.isNull() && p_data.contains(field.name())) {
    p_data.remove(field.name());
    return;
  }

  p_data.insert(field.name(), value);
}

const QVariant ASqlTable::getValue(const QString &column) {
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

const QHash<QString, QVariant> ASqlTable::getDataset() { return p_data; }

}; // namespace AntiquaCRM
