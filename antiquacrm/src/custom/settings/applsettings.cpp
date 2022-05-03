// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "applsettings.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QDir>

static const QString config_domain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_NAME));
  return str;
}

ApplSettings::ApplSettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope,
                config_domain(), ANTIQUACRM_NAME, parent) {

    setValue("application/name",ANTIQUACRM_NAME);
    setValue("application/version",ANTIQUACRM_VERSION);
}

const QString ApplSettings::sqlConnectioName() {
  QString str(ANTIQUACRM_CONNECTION_PREFIX);
  str.append(ANTIQUACRM_NAME);
  return str;
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

ApplSettings::~ApplSettings() {}
