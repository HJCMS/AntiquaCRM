// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerorders.h"

ProviderOrder::ProviderOrder(const QString &param, const QString &field,
                           const QVariant &value, const QString &group) {
  p_group = group;
  p_param = param;
  p_field = field;
  p_value = value;
}

ProviderOrder::ProviderOrder() : ProviderOrder{QString(), QString()} {}

void ProviderOrder::setGroup(const QString &p) { p_group = p; }

const QString ProviderOrder::group() { return p_group; }

void ProviderOrder::setParam(const QString &p) { p_param = p; }

const QString ProviderOrder::param() { return p_param; }

void ProviderOrder::setFieldname(const QString &f) { p_field = f; }

const QString ProviderOrder::fieldname() { return p_field; }

void ProviderOrder::setValue(const QVariant &v) { p_value = v; }

const QVariant ProviderOrder::value() { return p_value; }
