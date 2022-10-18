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
  return (db.isValid() ? db.record(name) : QSqlRecord());
}

ASqlDataQuery::ASqlDataQuery(const QString &tableName)
    : p_record{asql_table_record(tableName)} {}

ASqlDataQuery::ASqlDataQuery(const QSqlRecord &record) : p_record{record}, p_data{} {
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
  QSqlField field;
  if (!isValid())
    return field;

  for (int i = 0; i < p_record.count(); i++) {
    if (p_record.fieldName(i) == column) {
      field = p_record.field(i);
      break;
    }
  }
  return field;
}

const QMetaType ASqlDataQuery::getType(const QString &column) const {
  QSqlField field = getProperties(column);
  if (field.isValid()) {
    QVariant::Type _t = field.type();
    return QMetaType(QMetaType::type(QVariant::typeToName(_t)));
  }
  return QMetaType();
}

void ASqlDataQuery::setValue(const QString &column, const QVariant &value) {
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
