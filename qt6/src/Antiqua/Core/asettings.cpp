// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asettings.h"
#include "aglobal.h"

#include <QCoreApplication>
#include <QStandardPaths>

#ifndef ANTIQUACRM_CONFIG_MAILLER_KEY
#define ANTIQUACRM_CONFIG_MAILLER_KEY QString("dirs/mailappl")
#endif

namespace AntiquaCRM {

ASettings::ASettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(),
                ANTIQUACRM_NAME, parent) {
  setValue("name", ANTIQUACRM_NAME);
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

bool ASettings::check(const QString &configPath) {
  if (contains(configPath)) {
    return (!value(configPath).isNull());
  }
  return false;
}

const QDir ASettings::getArchivPath(const QString &section) {
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

  QString fallback = QStandardPaths::writableLocation(location);
  beginGroup("dirs");
  d.setPath(value(section, fallback).toString());
  endGroup();
  return d;
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

const QHash<QString, QVariant> &
ASettings::readGroupSection(const QString &group, const QString &section) {
  p_hash.clear();
  beginGroup(group + "/" + section);
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

const QStringList ASettings::pluginSearchFilter() {
#ifdef Q_OS_LINUX
  return QStringList("*.so");
#else
  return QStringList("*.dll");
#endif
}

const QDir ASettings::getTranslationDir() {
  QDir t(ANTIQUACRM_TRANSLATION_TARGET);
  t.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  t.setNameFilters(QStringList("*.qm"));
  t.setSorting(QDir::Name);
  return t;
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

const QDir ASettings::getPluginDir(const QString &subTarget) {
  QString pluginDir(ANTIQUACRM_PLUGIN_TARGET);
  if (!subTarget.isEmpty()) {
    pluginDir.append(QDir::separator());
    pluginDir.append(subTarget);
  }

  QDir t(pluginDir);
  t.setFilter(directoryFilter());
  t.setSorting(QDir::Name);
  t.setNameFilters(pluginSearchFilter());
  return t;
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
