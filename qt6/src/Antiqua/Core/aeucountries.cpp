// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aeucountries.h"

namespace AntiquaCRM {

AEUCountries::AEUCountries(QObject *parent)
    : QObject{parent}, QMap<QString, QString>{} {
  setObjectName("AEUCountries");
  insert("XX", tr("Non European Country"));
  insert("BE", tr("Belgium"));
  insert("BG", tr("Bulgaria"));
  insert("DK", tr("Denmark"));
  insert("DE", tr("Germany"));
  insert("EE", tr("Estonia"));
  insert("FI", tr("Finland"));
  insert("FR", tr("France"));
  insert("GR", tr("Greece"));
  insert("IE", tr("Ireland"));
  insert("IT", tr("Italy"));
  insert("HR", tr("Croatia"));
  insert("LV", tr("Latvia"));
  insert("LT", tr("Lithuania"));
  insert("LU", tr("Luxembourg"));
  insert("MT", tr("Malta"));
  insert("NL", tr("Netherlands"));
  insert("AT", tr("Austria"));
  insert("PL", tr("Poland"));
  insert("PT", tr("Portugal"));
  insert("RO", tr("Romania"));
  insert("SE", tr("Sweden"));
  insert("SK", tr("Slovakia"));
  insert("SI", tr("Slovenia"));
  insert("ES", tr("Spain"));
  insert("CZ", tr("Czech"));
  insert("HU", tr("Hungary"));
  insert("CY", tr("Cyprus"));
}

const QString AEUCountries::bcp47Name(const QString &country) {
  return key(country, "XX");
}

const QString AEUCountries::country(const QString &bcp47) {
  return contains(bcp47) ? value(bcp47) : QString();
}

} // namespace AntiquaCRM
