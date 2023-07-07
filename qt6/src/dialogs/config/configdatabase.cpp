// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdatabase.h"
#include "sslcaselecter.h"
#include "sslmode.h"

#include <QLabel>
#include <QLocale>
#include <QMetaType>
#include <QtWidgets>

ConfigDatabase::ConfigDatabase(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"database", QString(), parent} {
  setWindowTitle(getMenuEntry().value("title").toString());
  // Central Widget
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QString _info = tr("Database Connection settings to PostgreSQL server.");

  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  QLabel *lb_title = new QLabel(this);
  lb_title->setText(_info);
  layout->addWidget(lb_title);

  m_profil = new AntiquaCRM::AComboBox(this);
  m_profil->setObjectName("database_profile");
  m_profil->setWithoutDisclosures("default");
  m_profil->setToolTip(tr("Database profile"));
  layout->addWidget(m_profil);

  // Primary Connection Settings
  QGroupBox *connectionGroup = new QGroupBox(this);
  connectionGroup->setTitle(tr("Access Configuration"));
  QVBoxLayout *m_groupLayout1 = new QVBoxLayout(connectionGroup);

  _info = tr("Hostname of the server you want to connect to.\n"
             "If you are not using SSL, the IP address can also be added in "
             "this field.");
  pg_hostname = new AntiquaCRM::TextLine(connectionGroup);
  pg_hostname->setObjectName("pg_hostname");
  pg_hostname->setRequired(true);
  pg_hostname->setBuddyLabel(tr("Hostname"));
  pg_hostname->setInputToolTip(tr("Hostname/Address"));
  pg_hostname->setWhatsThisText(_info);
  m_groupLayout1->addWidget(pg_hostname);

  _info = tr("Name of the server database, you want to connect.");
  pg_database = new AntiquaCRM::TextLine(connectionGroup);
  pg_database->setObjectName("pg_database");
  pg_database->setRequired(true);
  pg_database->setBuddyLabel(tr("Database"));
  pg_database->setInputToolTip(tr("Databasename"));
  pg_database->setWhatsThisText(_info);
  m_groupLayout1->addWidget(pg_database);

  _info = tr("Loginname to the server database, you want to connect.");
  pg_username = new AntiquaCRM::TextLine(connectionGroup);
  pg_username->setObjectName("pg_username");
  pg_username->setRequired(true);
  pg_username->setBuddyLabel(tr("Username"));
  pg_username->setInputToolTip(tr("Login Username"));
  pg_username->setWhatsThisText(_info);
  m_groupLayout1->addWidget(pg_username);

  _info = tr("Login password for this database connection.");
  pg_password = new AntiquaCRM::TextLine(connectionGroup);
  pg_password->setObjectName("pg_password");
  pg_password->setRequired(true);
  pg_password->setBuddyLabel(tr("Password"));
  pg_password->setInputToolTip(tr("Database Password"));
  pg_password->setWhatsThisText(_info);
  pg_password->setPasswordInput(true);
  m_groupLayout1->addWidget(pg_password);
  // BEGIN:Singleline Layout
  QHBoxLayout *m_groupLayout1s = new QHBoxLayout;
  _info = tr("Port number to connect to the server. The default port address "
             "to a PostgreSQL server is 5432. However, if you connect from "
             "outside, you need the configured router port. To know this, ask "
             "your Network Administrator.");
  pg_port = new AntiquaCRM::NumEdit(connectionGroup);
  pg_port->setObjectName("pg_port");
  pg_port->setRequired(true);
  pg_port->setBuddyLabel(tr("Port"));
  pg_port->setInputToolTip(tr("Database connection port"));
  pg_port->setWhatsThisText(_info);
  pg_port->setRange(1024, 49152);
  pg_port->setValue(5432);
  m_groupLayout1s->addWidget(pg_port);
  _info = tr("Maximum waiting time before the application aborts the "
             "connection attempt.");
  pg_timeout = new AntiquaCRM::NumEdit(connectionGroup);
  pg_timeout->setObjectName("pg_timeout");
  pg_timeout->setRequired(true);
  pg_timeout->setBuddyLabel(tr("Timeout"));
  pg_timeout->setInputToolTip(tr("Timeout in seconds"));
  pg_timeout->setWhatsThisText(_info);
  pg_timeout->setRange(1, 30);
  pg_timeout->setValue(5);
  m_groupLayout1s->addWidget(pg_timeout);
  m_groupLayout1s->addStretch(1);
  m_groupLayout1->addLayout(m_groupLayout1s);
  // END:Singleline Layout

  connectionGroup->setLayout(m_groupLayout1);
  layout->addWidget(connectionGroup);

  // BEGIN:SSL/TLS Settings
  // https://www.postgresql.org/docs/current/libpq-ssl.html
  // https://www.postgresql.org/docs/current/ssl-tcp.html
  pg_ssl = new AntiquaCRM::GroupBoxEdit(this);
  pg_ssl->setBuddyLabel(tr("SSL/TLS Connection"));
  pg_ssl->setValue(true);

  QBoxLayout *m_sslLayout = pg_ssl->boxLayout();
  m_sslLayout->setContentsMargins(5, 5, 5, 5);
  m_sslLayout->setDirection(QBoxLayout::TopToBottom);

  _info = tr(
      "Certification Authority (CA) contains root or intermediate certificate. "
      "Together with your server certificate (issued specifically for your "
      "domain), these certificates complete the SSL chain of trust. The chain "
      "is required to verify the Authentication with the main application.");
  ssl_CA = new SslCaSelecter(pg_ssl);
  ssl_CA->setObjectName("ssl_CA");
  ssl_CA->setBuddyLabel(tr("Certification Authority"));
  ssl_CA->setInputToolTip(tr("CA Bundle listing"));
  ssl_CA->setWhatsThisText(_info);
  m_sslLayout->addWidget(ssl_CA);

  _info = tr("Common Name (CN) of the Server Certificate. "
             "Normally the Hostname of the SQL Server or from connected "
             "Dynamic DNS Client. It is important that this Name is equal "
             "to the Server certificate.");
  ssl_CN = new AntiquaCRM::TextLine(pg_ssl);
  ssl_CN->setObjectName("ssl_CN");
  ssl_CN->setBuddyLabel(tr("Common Name"));
  ssl_CN->setInputToolTip(tr("Common Name (CN)"));
  ssl_CN->setWhatsThisText(_info);
  ssl_CN->appendStretch(0);
  m_sslLayout->addWidget(ssl_CN);

  _info = tr("Exhibitor Certificate from SQL Server. "
             "This is needed for Certification Handshake mechanism. "
             "It depends to CA Issuers.");
  ssl_root_cert = new AntiquaCRM::SelectFile(pg_ssl);
  ssl_root_cert->setObjectName("ssl_root_cert");
  ssl_root_cert->setBuddyLabel(tr("Exhibitor Certificate"));
  ssl_root_cert->setInputToolTip(tr("Exhibitor Certificate from Server"));
  ssl_root_cert->setWhatsThisText(_info);
  m_sslLayout->addWidget(ssl_root_cert);

  _info = tr("For a connection to be known as SSL-secured, SSL usage must be "
             "configured on both the client and server before the connection "
             "is established. If it is only configured on the server, the "
             "client may send sensitive information (e.g. passwords) before "
             "knowing that the server requires high security. In this case, "
             "secure connections can be guaranteed by setting the SSL "
             "connection to „Required“, „Verify full“ or „Verify-CA“.");
  ssl_mode = new SslMode(pg_ssl);
  ssl_mode->setObjectName("ssl_mode");
  ssl_mode->setBuddyLabel(tr("SSL Mode"));
  ssl_mode->setInputToolTip(tr("SSL Protection Provided in Different Modes."));
  ssl_mode->setWhatsThisText(_info);
  ssl_mode->appendStretch(0);
  m_sslLayout->addWidget(ssl_mode);

  layout->addWidget(pg_ssl);

  _info = tr("Information about Secure SQL Connection you can found on ");
  _info.append("<a href='" + pgsqlClientAuthDocUrl().toString());
  _info.append("' target='_blank'>PostgreSQL - ");
  _info.append(tr("Client Authentication Documentation"));
  _info.append("</a>.");

  QLabel *infolink = new QLabel(this);
  infolink->setIndent(5);
  infolink->setWordWrap(true);
  infolink->setOpenExternalLinks(true);
  infolink->setText(_info);
  layout->addWidget(infolink);

  layout->addStretch(1);
  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);
}

const QUrl ConfigDatabase::pgsqlClientAuthDocUrl() {
  QUrl url;
  url.setScheme("https");
  url.setHost("www.postgresql.org");
  url.setPath("/docs/current/client-authentication.html");
  return url;
}

void ConfigDatabase::loadSectionConfig() {
  QString _profile = config->value("database_profile", "Default").toString();
  config->beginGroup("database");
  foreach (QString _group, config->childGroups()) {
    m_profil->addItem(_group, _group);
    if (_group == _profile)
      m_profil->setCurrentText(_profile);
  }
  config->endGroup();
  // Update
  _profile = m_profil->currentData().toString();
  config->beginGroup("database/" + _profile);
  QList<AntiquaCRM::AInputWidget *> _l =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _l.count(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _l.at(i);
    if (m_inp != nullptr && !m_inp->objectName().isEmpty()) {
      const QString _key = m_inp->objectName();
      QVariant _value = config->getValue(_key, m_inp->getType());
      if (!_value.isValid())
        continue;

      m_inp->setValue(_value);
    }
  }

  QFileInfo _f(config->getValue("ssl_bundle", ssl_CA->getType()).toString());
  if (_f.isReadable())
    ssl_CA->setBundle(_f.filePath());

  config->endGroup();
}

void ConfigDatabase::saveSectionConfig() {
  QString _profile = m_profil->currentData().toString();
  if (_profile.isEmpty())
    _profile = QString("Default");

  config->setValue("database_profile", _profile);
  config->beginGroup("database/" + _profile);
  QList<AntiquaCRM::AInputWidget *> _l =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _l.count(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _l.at(i);
    if (m_inp != nullptr && !m_inp->objectName().isEmpty()) {
      const QString _key = m_inp->objectName();
      QVariant _value = m_inp->getValue();
      if (!_value.isValid())
        config->remove(_key);
      else
        config->setValue(_key, _value);
    }
  }
  config->setValue("ssl_bundle", ssl_CA->getBundle());
  config->endGroup();
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigDatabase::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigDatabase::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "network-server-database");
  _o.insert("title", tr("Database"));
  _o.insert("tooltip", tr("Database configuration."));
  return _o;
}
