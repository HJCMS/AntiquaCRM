// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlsettings.h"

#include <AntiquaCRM>
#include <QDir>

namespace AntiquaCRM {

ASqlSettings::ASqlSettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(),
                ANTIQUACRM_NAME, parent) {
  setObjectName("antiquacrm_sqlsettings");
}

const QString ASqlSettings::configDomain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_NAME));
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_VERSION));
  return str;
}

const QString ASqlSettings::connectionName() {
  return QString(ANTIQUACRM_CONNECTION_DOMAIN);
}

void ASqlSettings::setProfile(const QString &name) { profile = name.trimmed(); }

const QString ASqlSettings::getProfile() { return profile; }

}; // namespace AntiquaCRM
