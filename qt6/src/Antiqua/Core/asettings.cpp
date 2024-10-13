// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asettings.h"
#include "aglobal.h"

#include <QCoreApplication>
#include <QLocale>
#include <QStandardPaths>

namespace AntiquaCRM
{

ASettings::ASettings(QObject* parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(), ANTIQUACRM_NAME,
                parent) {
  setValue("name", ANTIQUACRM_NAME);
  setValue("version", ANTIQUACRM_VERSION);
}

ASettings::ASettings(const QString& applName, QObject* parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(), applName, parent) {
  setValue("name", applName);
  setValue("version", ANTIQUACRM_VERSION);
}

const QString ASettings::configDomain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_NAME));
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_VERSION));
  return str;
}

bool ASettings::check(const QString& pkey) const {
  if (contains(pkey)) {
    return (!value(pkey).isNull());
  }
  return false;
}

const QVariant ASettings::getValue(const QString& key, const QMetaType& type) const {
  if (key.isEmpty())
    return QVariant();

  QVariant _value = QSettings::value(key);
  switch (type.id()) {
    case (QMetaType::Bool):
      return _value.toBool();

    case (QMetaType::Int):
    case (QMetaType::Long):
    case (QMetaType::LongLong):
      return _value.toInt();

    case (QMetaType::Float):
      return _value.toFloat();

    case (QMetaType::Double):
      return _value.toDouble();

    default:
      break;
  };

  return _value;
}

const QDir ASettings::getArchivPath(const QString& section) {
  QDir d;
  QStandardPaths::StandardLocation location;
  if (section.contains("import"))
    location = QStandardPaths::DownloadLocation;
  else if (section.contains("images"))
    location = QStandardPaths::PicturesLocation;
  else if (section.contains("media"))
    location = QStandardPaths::MusicLocation;
  else if (section.contains("temp"))
    location = QStandardPaths::TempLocation;
  else
    location = QStandardPaths::DocumentsLocation;

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << section << " = " << location;
#endif

  QString fallback = QStandardPaths::writableLocation(location);
  beginGroup(ANTIQUACRM_ARCHIVE_CONFIG_DIRS);
  d.setPath(value(section, fallback).toString());
  endGroup();
  return d;
}

const QVariant ASettings::groupValue(const QString& group, const QString& key,
                                     const QVariant& fallback) {
  if (group.isEmpty() || key.isEmpty())
    return fallback;

  QVariant _retval;
  beginGroup(group);
  _retval = value(key, fallback);
  endGroup();
  return _retval;
}

const QHash<QString, QVariant> ASettings::payment() {
  QLocale _lc = QLocale::system();
  QHash<QString, QVariant> _d; // defaults
  _d.insert("currency", _lc.currencySymbol(QLocale::CurrencySymbol));
  _d.insert("price_limit_lowest", 1.00); // 1,- €
  _d.insert("price_limit_normal", 5.00); // 5,- €
  _d.insert("vat_normal", 19.00);        // 19%
  _d.insert("vat_reduced", 7.00);        // 7%

  QHash<QString, QVariant> _hash;
  beginGroup("payment");
  QHashIterator<QString, QVariant> it(_d);
  while (it.hasNext()) {
    it.next();
    _hash.insert(it.key(), value(it.key(), it.value()));
  }
  endGroup();
  _d.clear();
  return _hash;
}

QDir::Filters ASettings::directoryFilter() {
  return (QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
}

const QStringList ASettings::pluginSearchFilter() {
#ifdef Q_OS_LINUX
  return QStringList({"*.a", "*.so"});
#else
  return QStringList("*.dll");
#endif
}

const QDir ASettings::getPluginDir(const QString& target) {
  QString _path(ANTIQUACRM_PLUGIN_TARGET);
  if (!target.isEmpty()) {
    _path.append(QDir::separator());
    _path.append(target);
  }

  QDir t(_path);
  t.setFilter(directoryFilter());
  t.setSorting(QDir::Name);
  t.setNameFilters(pluginSearchFilter());
  return t;
}

const QDir ASettings::getTranslationDir() {
  QDir t(ANTIQUACRM_TRANSLATION_TARGET);
  t.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  t.setNameFilters(QStringList("*.qm"));
  t.setSorting(QDir::Name);
  return t;
}

const QDir ASettings::getDataDir(const QString& name) {
  QString p(ANTIQUACRM_DATA_TARGET);
  p.append(QDir::separator());
  if (!name.isEmpty()) {
    p.append(QDir::separator());
    p.append(name);
  }

  QDir t(p);
  QStringList filters({"*.xml", "*.sql", "*.json", "*.txt", "*.qcss"});
  t.setNameFilters(filters);
  t.setFilter(directoryFilter());
  t.setSorting(QDir::Name);
  return t.isReadable() ? t : QDir(ANTIQUACRM_DATA_TARGET);
}

const QDir ASettings::getUserDataDir() {
  QString data = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  QDir d(data);
  if (!d.mkpath(d.path())) {
    qWarning("DataLocation: Permission denied!");
  }
  d.setFilter(directoryFilter());
  d.setSorting(QDir::Name);
  return d;
}

const QDir ASettings::getUserTempDir() {
  QDir d(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
  if (!d.mkpath(d.path())) {
    qWarning("CacheLocation: Permission denied!");
  }
  d.setFilter(directoryFilter());
  d.setSorting(QDir::Name);
  return d;
}

}; // namespace AntiquaCRM
