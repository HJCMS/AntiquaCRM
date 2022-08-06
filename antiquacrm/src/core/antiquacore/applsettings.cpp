// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "applsettings.h"
#include "antiqua_global.h"

#include <QApplication>
#include <QDebug>

static const QString config_domain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_NAME));
  return str;
}

ApplSettings::ApplSettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, config_domain(),
                ANTIQUACRM_NAME, parent) {
  beginGroup("application");
  setValue("name", ANTIQUACRM_NAME);
  setValue("version", ANTIQUACRM_VERSION);
  endGroup();
}

const QString ApplSettings::sqlConnectioName() {
  return QString(ANTIQUACRM_CONNECTION_DOMAIN);
}

bool ApplSettings::needAssistant() {
  QStringList requires("postgresql/database");
  requires << "postgresql/hostname";
  requires << "postgresql/username";
  requires << "postgresql/password";
  requires << "postgresql/port";
  foreach (QString k, requires) {
    if (!contains(k) || value(k).isNull()) {
      return true;
    }
  }
  return false;
}

const QHash<QString, QVariant> &
ApplSettings::readGroupConfig(const QString &group) {
  p_hash.clear();
  beginGroup(group);
  foreach (QString key, allKeys()) {
    p_hash.insert(key, value(key));
  }
  endGroup();
  return p_hash;
}

void ApplSettings::writeGroupConfig(const QString &group,
                                    const QHash<QString, QVariant> &dataset) {
  if (group.isEmpty())
    return;

  beginGroup(group);
  QHashIterator<QString, QVariant> i(dataset);
  while (i.hasNext()) {
    i.next();
    setValue(i.key(), i.value());
    // qDebug() << __FUNCTION__ << i.key() << ": " << i.value() << Qt::endl;
  }
  endGroup();
}

const QDir ApplSettings::getDataTarget(const QString &name) {
#ifdef ANTIQUA_LSB_TARGETS
  QString p(ANTIQUACRM_DATA_TARGET);
#else
  QString p(QDir::currentPath());
#endif
  QDir t(p);
  return name.isEmpty() ? t : QDir(t.filePath(name));
}

ApplSettings::~ApplSettings() {}
