// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aeuropeancountries.h"

#include <QObject>

namespace AntiquaCRM {

AEuropeanCountries::AEuropeanCountries() : QHash<QString, QString>{} {
  insert("BE", QObject::tr("Belgium"));
  insert("BG", QObject::tr("Bulgaria"));
  insert("DK", QObject::tr("Denmark"));
  insert("DE", QObject::tr("Germany"));
  insert("EE", QObject::tr("Estonia"));
  insert("FI", QObject::tr("Finland"));
  insert("FR", QObject::tr("France"));
  insert("GR", QObject::tr("Greece"));
  insert("IE", QObject::tr("Ireland"));
  insert("IT", QObject::tr("Italy"));
  insert("HR", QObject::tr("Croatia"));
  insert("LV", QObject::tr("Latvia"));
  insert("LT", QObject::tr("Lithuania"));
  insert("LU", QObject::tr("Luxembourg"));
  insert("MT", QObject::tr("Malta"));
  insert("NL", QObject::tr("Netherlands"));
  insert("AT", QObject::tr("Austria"));
  insert("PL", QObject::tr("Poland"));
  insert("PT", QObject::tr("Portugal"));
  insert("RO", QObject::tr("Romania"));
  insert("SE", QObject::tr("Sweden"));
  insert("SK", QObject::tr("Slovakia"));
  insert("SI", QObject::tr("Slovenia"));
  insert("ES", QObject::tr("Spain"));
  insert("CZ", QObject::tr("Czech"));
  insert("HU", QObject::tr("Hungary"));
  insert("CY", QObject::tr("Cyprus"));
}

}; // namespace AntiquaCRM
