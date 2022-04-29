// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datafield.h"

DataField::DataField() {
  p_field = QString();
  p_type = -1;
  p_notnull = false;
  p_value = QVariant();
}

DataField::DataField(const QString &field, int type, bool notnull,
                     const QVariant &data) {
  p_field = field;
  p_type = type;
  p_notnull = notnull;
  p_value = data;
}

void DataField::setField(const QString &field) { p_field = field; }

const QString DataField::field() { return p_field; }

void DataField::setType(int type) { p_type = type; }

int DataField::type() { return p_type; }

void DataField::setRequired(bool b) { p_notnull = b; }

bool DataField::isRequired() { return p_notnull; }

void DataField::setValue(const QVariant &value) { p_value = value; }

const QVariant DataField::value() { return p_value; }
