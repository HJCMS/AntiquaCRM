// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "settings.h"

#include <AGlobal>
#include <QCoreApplication>
#include <QDir>

Settings::Settings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(),
                "antiquacmd", parent) {
  setValue("name", applicationName());
  setValue("version", ANTIQUACRM_VERSION);
}

const QString Settings::configDomain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_NAME));
  str.append(QDir::separator());
  str.append(QString::fromUtf8(HJCMS_CONFIG_VERSION));
  return str;
}
