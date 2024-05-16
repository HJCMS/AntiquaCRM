// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGDATABASE_H
#define ANTIQUACRM_DIALOGS_CONFIGDATABASE_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class DatabaseProfile;
class SslCaSelecter;
class SslMode;

class ConfigDatabase final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  // Database connectionName
  const QString p_connection_id;
  // Connection Settings
  DatabaseProfile *m_profil;
  AntiquaCRM::TextLine *pg_hostname;
  AntiquaCRM::TextLine *pg_database;
  AntiquaCRM::TextLine *pg_username;
  AntiquaCRM::TextLine *pg_password;
  AntiquaCRM::NumEdit *pg_port;
  AntiquaCRM::NumEdit *pg_timeout;
  // SSL/TLS Settings
  SslCaSelecter *ssl_CA;
  AntiquaCRM::TextLine *ssl_CN;
  AntiquaCRM::SelectFile *ssl_root_cert;
  SslMode *ssl_mode;

  static const QUrl pgSqlAuthDocUrl();

  bool resetInput();

  bool loadProfile(const QString &id);

private Q_SLOTS:
  void testConnection();
  void updateProfile();

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigDatabase(QWidget *parent = nullptr);
  virtual ~ConfigDatabase();
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGDATABASE_H
