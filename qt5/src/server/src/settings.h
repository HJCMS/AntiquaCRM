// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_SETTINGS_H
#define ANTIQUACMD_SETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

class Settings : public QSettings {
  Q_OBJECT

public:
  explicit Settings(QObject *parent = nullptr);

  /**
   * @brief AntiquaCRM Configuration Domain
   * This is needed to describe the configuration paths
   */
  static const QString configDomain();

};

#endif // ANTIQUACMD_SETTINGS_H
