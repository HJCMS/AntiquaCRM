// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aeucountries.h"

namespace AntiquaCRM {

AEUCountries::AEUCountries(const QLocale &locale)
    : QLocale{locale}, QMap<QString, QString>{} {
  insert("XX", QString());
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

const QString AEUCountries::isoCode(const QString &country) {
  return key(country, "XX");
}

const QString AEUCountries::countryName(const QString &iso) {
  return contains(iso) ? value(iso) : QString();
}

const QList<QLocale::Country> AEUCountries::europeanUnion() {
  QList<QLocale::Country> _eul;
  _eul.append(QLocale::Austria);
  _eul.append(QLocale::Belgium);
  _eul.append(QLocale::Bulgaria);
  _eul.append(QLocale::Croatia);
  _eul.append(QLocale::Cyprus);
  _eul.append(QLocale::Czechia);
  _eul.append(QLocale::Denmark);
  _eul.append(QLocale::Estonia);
  _eul.append(QLocale::Finland);
  _eul.append(QLocale::France);
  _eul.append(QLocale::Germany);
  _eul.append(QLocale::Greece);
  _eul.append(QLocale::Hungary);
  _eul.append(QLocale::Ireland);
  _eul.append(QLocale::Italy);
  _eul.append(QLocale::Latvia);
  _eul.append(QLocale::Lithuania);
  _eul.append(QLocale::Luxembourg);
  _eul.append(QLocale::Malta);
  _eul.append(QLocale::Netherlands);
  _eul.append(QLocale::Poland);
  _eul.append(QLocale::Portugal);
  _eul.append(QLocale::Romania);
  _eul.append(QLocale::Slovakia);
  _eul.append(QLocale::Slovenia);
  _eul.append(QLocale::Spain);
  _eul.append(QLocale::Sweden);
  return _eul;
}

} // namespace AntiquaCRM
