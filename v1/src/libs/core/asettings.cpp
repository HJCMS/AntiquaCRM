// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asettings.h"
#include "aglobal.h"

#include <QApplication>
#include <QStandardPaths>

namespace AntiquaCRM {

ASettings::ASettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(),
                ANTIQUACRM_NAME, parent) {
  beginGroup("application");
  setValue("name", ANTIQUACRM_NAME);
  setValue("version", ANTIQUACRM_VERSION);
  endGroup();
}

const QString ASettings::configDomain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_NAME));
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_VERSION));
  return str;
}

const QVariant ASettings::defaultValue(const QString &key,
                                       const QVariant &val) {
  setValue(key, val);
  return value(key, val);
}

const QHash<QString, QVariant> &
ASettings::readGroupConfig(const QString &group) {
  p_hash.clear();
  beginGroup(group);
  foreach (QString key, allKeys()) {
    p_hash.insert(key, value(key));
  }
  endGroup();
  return p_hash;
}

void ASettings::writeGroupConfig(const QString &group,
                                 const QHash<QString, QVariant> &dataset) {
  if (group.isEmpty())
    return;

  beginGroup(group);
  QHashIterator<QString, QVariant> i(dataset);
  while (i.hasNext()) {
    i.next();
    setValue(i.key(), i.value());
  }
  endGroup();
}

QDir::Filters ASettings::directoryFilter() {
  return (QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
}

const QDir ASettings::getDataDir(const QString &name) {
  QString p(ANTIQUACRM_DATA_TARGET);
  p.append(QDir::separator());
  p.append(name);
  QDir t(p);
  QStringList filters({"*.xml", "*.sql", "*.json"});
  t.setNameFilters(filters);
  t.setFilter(directoryFilter());
  t.setSorting(QDir::Name);
  return t.isReadable() ? t : QDir(ANTIQUACRM_DATA_TARGET);
}

const QDir ASettings::getPluginDir() {
  QDir t(ANTIQUACRM_PLUGIN_TARGET);
  t.setFilter(directoryFilter());
  t.setSorting(QDir::Name);
#ifdef Q_OS_LINUX
  t.setNameFilters(QStringList("*.so"));
#else
  t.setNameFilters(QStringList("*.dll"));
#endif
  return t;
}

const QDir ASettings::getUserDataDir() {
  QString data = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
  QDir d(data);
  if (!d.mkpath(d.path())) {
    qWarning("Permission denied!");
  }
  d.setFilter(directoryFilter());
  d.setSorting(QDir::Name);
  return d;
}

const QDir ASettings::getUserTempDir() {
  QString data =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  QDir d(data);
  d.setFilter(directoryFilter());
  d.setSorting(QDir::Name);
  return d;
}

}; // namespace AntiquaCRM
