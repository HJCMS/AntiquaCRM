// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLSETTINGS_H
#define ANTIQUACRM_SQLSETTINGS_H

#include <AGlobal>
#include <ASettings>
#include <QByteArray>
#include <QObject>
#include <QSettings>
#include <QString>

namespace AntiquaCRM {

class ASqlProfile;

/**
 * @brief This class provides sql settings functions
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ASqlSettings final : public ASettings {
  Q_OBJECT
  Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY
                 sendProfileChanged)

private:
  QString profile;
  const QString groupPath();
  const QByteArray toRealm(const QString &pass);
  const QString fromRealm(const QByteArray &array);

Q_SIGNALS:
  void sendProfileChanged();

public:
  explicit ASqlSettings(const QString &name, QObject *parent = nullptr);
  explicit ASqlSettings(QObject *parent = nullptr);

  /**
   * @brief Global Connectionname.
   */
  static const QString connectionName();

  /**
   * @brief get current Connection Profile
   */
  const ASqlProfile connectionProfile();

  /**
   * @brief set Connection Profile.
   */
  void setProfile(const QString &name);

  /**
   * @brief get current Connection Profile.
   */
  const QString getProfile();

  /**
   * @brief the List from all configured Profiles
   */
  const QStringList profiles();

  /**
   * @brief set Value with Section-Key to current Profile.
   */
  void setParam(const QString &key, const QVariant &value);

  /**
   * @brief get Value with Section-Key from current Profile.
   */
  const QVariant getParam(const QString &key);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLSETTINGS_H
