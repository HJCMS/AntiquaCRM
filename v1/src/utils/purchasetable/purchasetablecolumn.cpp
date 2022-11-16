// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetablecolumn.h"

#include <AntiquaCRM>

PurchaseTableColumn::PurchaseTableColumn() : p_f(QString()), p_n(QString()) {}

PurchaseTableColumn::PurchaseTableColumn(const QString &f, const QString &n)
    : p_f(f), p_n(n) {}

const QString PurchaseTableColumn::field() { return p_f; }

const QString PurchaseTableColumn::name() { return p_n; }

const QMetaType::Type PurchaseTableColumn::type() {
  if (p_f.contains("a_"))
    return AntiquaCRM::AProviderOrder::articleKeys().value(p_f);
  else
    return QMetaType::QString;
}
