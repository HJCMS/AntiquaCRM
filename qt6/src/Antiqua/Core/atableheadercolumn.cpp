// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "atableheadercolumn.h"
#include "aproviderorders.h"

#include <QDebug>

namespace AntiquaCRM {

ATableHeaderColumn::ATableHeaderColumn() : p_f(QString()), p_n(QString()) {}

ATableHeaderColumn::ATableHeaderColumn(const QSqlField &field,
                                       const QString &displayname)
    : p_f(field.name()), p_n(displayname) {}

ATableHeaderColumn::ATableHeaderColumn(const QString &f, const QString &n)
    : p_f(f), p_n(n) {}

const QString ATableHeaderColumn::field() { return p_f; }

const QString ATableHeaderColumn::name() { return p_n; }

QMetaType::Type ATableHeaderColumn::type(HeaderSection section) {
  if (p_f.isEmpty())
    return QMetaType::UnknownType;

  if (section == ARTICLE)
    return AntiquaCRM::AProviderOrder::articleKeys().value(p_f);
  else if (section == ORDER)
    return AntiquaCRM::AProviderOrder::orderKeys().value(p_f);
  else if (section == CUSTOMER)
    return AntiquaCRM::AProviderOrder::customerKeys().value(p_f);
  else
    return QMetaType::UnknownType;
}

}; // namespace AntiquaCRM
