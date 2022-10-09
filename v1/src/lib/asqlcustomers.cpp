// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcustomers.h"
#include "asqlcore.h"

#include <QVariant>

namespace AntiquaCRM {

static const QSqlRecord customersRecord(QObject *obj = nullptr) {
  ASqlCore *m_sql = new ASqlCore(obj);
  return m_sql->record("customers");
}

ASqlCustomers::ASqlCustomers(QObject *parent)
    : QObject{parent}, p_record(customersRecord(parent)) {
  setObjectName("antiqua_sql_customers");
}

const QString ASqlCustomers::trim(const QString &str) {
  QString out = str;
  out.replace(QRegExp("\\s+"), " ");
  return out.trimmed();
}

const QString ASqlCustomers::toPhone(const QString &phone) {
  QString out = phone.trimmed();
  out.replace(QRegExp("\\D+"), "");
  return out.trimmed();
}

const QRegExp ASqlCustomers::mailRegExp() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

const QRegExp ASqlCustomers::phoneRegExp() {
  QRegExp reg;
  reg.setPattern("^([\\d]{6}[\\d]+)$");
  return reg;
}

const QStringList ASqlCustomers::fieldNames() const {
  QStringList fields;
  if (p_record.count() < 1)
    return fields;

  for (int i = 0; i < p_record.count(); i++) {
    fields << p_record.fieldName(i);
  }

  return fields;
}

const QMetaType ASqlCustomers::getType(const QString &fieldName) const {
  for (int i = 0; i < p_record.count(); i++) {
    if (p_record.fieldName(i) == fieldName) {
      QVariant::Type _t = p_record.field(i).type();
      return QMetaType(QMetaType::type(QVariant::typeToName(_t)));
    }
  }
  return QMetaType();
}

void ASqlCustomers::setValue(const QString &fieldName, const QVariant &value) {
  if (p_record.count() < 1)
    return;

  QSqlField field;
  for (int i = 0; i < p_record.count(); i++) {
    if (p_record.fieldName(i) == fieldName) {
      field = p_record.field(i);
      break;
    }
  }

  if (!field.isValid())
    return;

  if (field.type() != value.type()) {
    qWarning("Invalid type rejected! Field:'%s' Require: %s",
             qPrintable(fieldName), QVariant(field.type()).typeName());
    return;
  }

  if (field.requiredStatus() == QSqlField::Required && value.isNull()) {
    if (field.defaultValue().isNull()) {
      qWarning("Invalid value! Field:'%s' is required and can't null.",
               qPrintable(fieldName));
      return;
    }
    p_data.insert(field.name(), field.defaultValue());
    return;
  }

  if (field.type() == QVariant::String && field.length() > 0) {
    if (value.toString().length() > field.length()) {
      qWarning("Invalid datasize! '%s' max length is '%d'",
               qPrintable(fieldName), field.length());
      return;
    }
  }

  if (value.isNull() && p_data.contains(field.name())) {
    p_data.remove(field.name());
    return;
  }

  p_data.insert(field.name(), value);
}

const QVariant ASqlCustomers::getValue(const QString &fieldName) {
  if (p_record.count() < 1)
    return QVariant();

  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == fieldName)
      return it.value();
  }
  return QVariant();
}

const QHash<QString, QVariant> ASqlCustomers::getDataset() { return p_data; }

}; // namespace AntiquaCRM
