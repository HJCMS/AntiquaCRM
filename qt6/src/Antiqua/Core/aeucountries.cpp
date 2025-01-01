// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aeucountries.h"

namespace AntiquaCRM
{

#ifndef FALLBACK_CODE
#  define FALLBACK_CODE QString("XX")
#endif

AEUCountries::AEUCountries(const QLocale& locale) : QLocale{locale}, QMap<QString, QString>{} {
  insert(FALLBACK_CODE, QString());
  QListIterator<QLocale::Country> it(europeanUnion());
  while (it.hasNext()) {
    QLocale _c(QLocale::AnyLanguage, it.next());
    QString _iso = _c.name().toUpper().trimmed();
    if (_iso.contains("-")) {
      _iso = _iso.split("-").last();
    } else if (_iso.contains("_")) {
      _iso = _iso.split("_").last();
    }
    insert(_iso, territoryToString(_c.territory()));
  }
}

const QString AEUCountries::isoCode(const QString& country) {
  return key(country, FALLBACK_CODE);
}

const QString AEUCountries::countryName(const QString& iso) {
  return contains(iso) ? value(iso) : QString();
}

const QList<QLocale::Country> AEUCountries::europeanUnion() {
  QList<QLocale::Country> _l;
  _l.append(QLocale::Austria);
  _l.append(QLocale::Belgium);
  _l.append(QLocale::Bulgaria);
  _l.append(QLocale::Croatia);
  _l.append(QLocale::Cyprus);
  _l.append(QLocale::Czechia);
  _l.append(QLocale::Denmark);
  _l.append(QLocale::Estonia);
  _l.append(QLocale::Finland);
  _l.append(QLocale::France);
  _l.append(QLocale::Germany);
  _l.append(QLocale::Greece);
  _l.append(QLocale::Hungary);
  _l.append(QLocale::Ireland);
  _l.append(QLocale::Italy);
  _l.append(QLocale::Latvia);
  _l.append(QLocale::Lithuania);
  _l.append(QLocale::Luxembourg);
  _l.append(QLocale::Malta);
  _l.append(QLocale::Netherlands);
  _l.append(QLocale::Poland);
  _l.append(QLocale::Portugal);
  _l.append(QLocale::Romania);
  _l.append(QLocale::Slovakia);
  _l.append(QLocale::Slovenia);
  _l.append(QLocale::Spain);
  _l.append(QLocale::Sweden);
  return _l;
}

} // namespace AntiquaCRM
