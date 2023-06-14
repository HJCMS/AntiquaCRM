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
 * @class ASqlSettings
 * @brief This class provides sql settings functions
 *
 * This class is used to Read Database Configuration Profiles on Application
 * start up.
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASqlSettings final : public ASettings {
  Q_OBJECT
  Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY
                 sendProfileChanged)

private:
  /**
   * @brief profile
   */
  QString profile;

  /**
   * @brief profilpath
   */
  const QString groupPath();

  /**
   * @param pass - password
   */
  const QByteArray toRealm(const QString &pass);

  /**
   * @param array - realm
   */
  const QString fromRealm(const QByteArray &array);

Q_SIGNALS:
  /**
   * @brief NOTIFY profile
   *
   * This Signal is emitted when Profile has changed.
   */
  void sendProfileChanged();

public:
  /**
   * @param parent - parent object
   */
  explicit ASqlSettings(QObject *parent = nullptr);

  /**
   * @brief Global Connectionname.
   * @return @ref ANTIQUACRM_CONNECTION_DOMAIN
   */
  static const QString connectionName();

  /**
   * @brief get current Connection Profile
   *
   * This function returns the current Database configuration @ref
   * AntiquaCRM::ASqlProfile.
   */
  const AntiquaCRM::ASqlProfile connectionProfile();

  /**
   * @brief WRITE profile property
   * @param name  - Profile name
   *
   * This function is used to change profile Property.
   */
  void setProfile(const QString &name);

  /**
   * @brief READ profile property
   *
   * This function is used to read current profile Property.
   */
  const QString getProfile();

  /**
   * @brief A List from all configured Profiles
   */
  const QStringList profiles();

  /**
   * @brief Write value with Key to current selected Profile.
   * @param key   - Key in Database profile config group
   * @param value - Value
   */
  void setParam(const QString &key, const QVariant &value);

  /**
   * @brief Read value with Key from current selected Profile.
   * @param key - Key in Database profile config group
   */
  const QVariant getParam(const QString &key);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLSETTINGS_H
