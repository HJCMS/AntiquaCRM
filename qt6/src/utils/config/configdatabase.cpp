// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdatabase.h"
#include "databaseprofile.h"
#include "sslcaselecter.h"
#include "sslmode.h"

#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QMetaType>
#include <QSqlDatabase>
#include <QSslConfiguration>
#include <QtWidgets>

ConfigDatabase::ConfigDatabase(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{QString(), parent},
      p_connection_id{"antiquacrm_db_connection_test"} {
  setObjectName("configdatabase");
  setWindowTitle(getMenuEntry().value("title").toString());

  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QString _info = tr("Database Connection settings to PostgreSQL server.");

  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  QLabel *lb_title = new QLabel(this);
  lb_title->setText(_info);
  layout->addWidget(lb_title);

  // DatabaseProfile
  m_profil = new DatabaseProfile(config, m_central);
  layout->addWidget(m_profil);

  // BEGIN:Primary Connection Settings
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
  // END:Primary Connection Settings

  // BEGIN:SSL/TLS Settings
  QGroupBox *m_sslGroup = new QGroupBox(this);
  m_sslGroup->setObjectName("pg_ssl");
  m_sslGroup->setTitle(tr("Required SSL/TLS Settings"));

  QBoxLayout *m_sslLayout = new QBoxLayout(QBoxLayout::TopToBottom, m_sslGroup);
  m_sslLayout->setContentsMargins(5, 5, 5, 5);

  _info = tr(
      "Certification Authority (CA) contains root or intermediate certificate. "
      "Together with your server certificate (issued specifically for your "
      "domain), these certificates complete the SSL chain of trust. The chain "
      "is required to verify the Authentication with the main application.");
  ssl_CA = new SslCaSelecter(m_sslGroup);
  ssl_CA->setObjectName("ssl_CA");
  ssl_CA->setBuddyLabel(tr("Certification Authority"));
  ssl_CA->setInputToolTip(tr("CA Bundle listing"));
  ssl_CA->setWhatsThisText(_info);
  m_sslLayout->addWidget(ssl_CA);

  _info = tr("Common Name (CN) of the Server Certificate. "
             "Normally the Hostname of the SQL Server or from connected "
             "Dynamic DNS Client. It is important that this Name is equal "
             "to the Server certificate.");
  ssl_CN = new AntiquaCRM::TextLine(m_sslGroup);
  ssl_CN->setObjectName("ssl_CN");
  ssl_CN->setBuddyLabel(tr("Common Name"));
  ssl_CN->setInputToolTip(tr("Common Name (CN)"));
  ssl_CN->setWhatsThisText(_info);
  ssl_CN->appendStretch(0);
  m_sslLayout->addWidget(ssl_CN);

  _info = tr("Exhibitor Certificate from SQL Server. This is needed for "
             "Certification Handshake mechanism. It depends to CA Issuers. If "
             "not know, add Alternative the CA-Issuer Certificate.");
  ssl_root_cert = new AntiquaCRM::SelectFile(m_sslGroup);
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
  ssl_mode = new SslMode(m_sslGroup);
  ssl_mode->setObjectName("ssl_mode");
  ssl_mode->setBuddyLabel(tr("SSL Mode"));
  ssl_mode->setInputToolTip(tr("SSL Protection Provided in Different Modes."));
  ssl_mode->setWhatsThisText(_info);
  ssl_mode->appendStretch(0);
  m_sslLayout->addWidget(ssl_mode);
  layout->addWidget(m_sslGroup);
  // END:SSL/TLS Settings

  // Final Info Widgets
  _info =
      tr("For more Information about Secure SQL Connection visit PostgreSQL");
  _info.prepend("<p>");
  _info.append(" - <a href='" + pgSqlAuthDocUrl().toString());
  _info.append("' target='_blank'>");
  _info.append(tr("Client Authentication Documentation"));
  _info.append("</a>.</p>");
  // GDPR https://gdpr.eu/
  _info.append("<p>");
  _info.append(tr("As part of the GDPR (General Data Protection Regulation), "
                  "SSL/TLS is always active."));
  _info.append(" ");
  _info.append(tr("Complete guide to <a href='https://gdpr.eu/' "
                  "target='_blank'>GDPR</a> compliance."));
  _info.append("</p>");

  QLabel *m_info_link = new QLabel(this);
  m_info_link->setIndent(5);
  m_info_link->setWordWrap(true);
  m_info_link->setOpenExternalLinks(true);
  m_info_link->setText(_info);
  layout->addWidget(m_info_link);

  layout->addStretch(1);
  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);

  connect(m_profil, SIGNAL(sendUpdateProfile()), SLOT(updateProfile()));
  connect(m_profil, SIGNAL(sendStartTest()), SLOT(testConnection()));
}

ConfigDatabase::~ConfigDatabase() {
  if (QSqlDatabase::contains(p_connection_id))
    QSqlDatabase::removeDatabase(p_connection_id);
}

const QUrl ConfigDatabase::pgSqlAuthDocUrl() {
  QUrl url;
  url.setScheme("https");
  url.setHost("www.postgresql.org");
  url.setPath("/docs/current/client-authentication.html");
  return url;
}

bool ConfigDatabase::resetInput() {
  QListIterator<AntiquaCRM::AInputWidget *> it(
      findChildren<AntiquaCRM::AInputWidget *>(QString()));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *inp = it.next();
    if (inp != nullptr)
      inp->reset();
  }
  return true;
}

bool ConfigDatabase::loadProfile(const QString &id) {
  QString _group = QString("database/%1").arg(id);
  config->beginGroup(_group);
  // qInfo("Profile:'database/%s'!", qPrintable(id));
  QList<AntiquaCRM::AInputWidget *> _l =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _l.count(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _l.at(i);
    if (m_inp != nullptr && !m_inp->objectName().isEmpty()) {
      const QString _key = m_inp->objectName();
      QVariant _value = config->getValue(_key, m_inp->getType());
      if (!_value.isValid()) {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << id << _key << _value;
#endif
        continue;
      }

      m_inp->setValue(_value);
    }
  }

  if (ssl_CA->getBundle().isEmpty()) {
    QFileInfo _f(config->getValue("ssl_bundle", ssl_CA->getType()).toString());
    if (_f.isReadable())
      ssl_CA->setBundle(_f.filePath());
  }

  config->endGroup();
  return true;
}

/*
 * https://www.postgresql.org/docs/current/libpq-ssl.html
 * https://www.postgresql.org/docs/current/ssl-tcp.html
 */
void ConfigDatabase::testConnection() {
  QSqlDatabase _db;
  if (QSqlDatabase::contains(p_connection_id)) {
    _db = QSqlDatabase::database(p_connection_id, false);
  } else {
    _db = QSqlDatabase::addDatabase("QPSQL", p_connection_id);
  }
  _db.setHostName(pg_hostname->getValue().toString());
  _db.setDatabaseName(pg_database->getValue().toString());
  _db.setPort(pg_port->getValue().toInt());

  QStringList _o; // Options
  _o << QString("connect_timeout=%1").arg(pg_timeout->getValue().toString());
  _o << QString("application_name=%1").arg(p_connection_id);
  _o << QString("target_session_attrs=read-write");
  QString _root_cert = ssl_root_cert->getValue().toString();
  if (!_root_cert.isEmpty()) {
    _o << QString("sslsni=1");
    _o << QString("sslmode=%1").arg(ssl_mode->getValue().toString());
    _o << QString("ssl_min_protocol_version=%1").arg("TLSv1.2");
    _o << QString("sslrootcert=%1").arg(_root_cert);
  } else {
    qWarning("Missing SSL/TLS Configuration!");
  }
  _db.setConnectOptions(_o.join(";"));

  // Append to global ca-bundle
  QSslConfiguration _ssl = QSslConfiguration::defaultDtlsConfiguration();
  _ssl.addCaCertificate(ssl_CA->getCert());
  _ssl.setProtocol(QSsl::TlsV1_2OrLater);

  // Realm
  const QString _user = pg_username->getValue().toString();
  const QString _pass = QByteArray::fromBase64(
      pg_password->getValue().toByteArray(), QByteArray::Base64UrlEncoding);

  QStringList _ret;
  if (_db.open(_user, _pass)) {
    _ret << "<b>" + tr("Connection successfully!") + "</b>";
  } else {
    _ret << "<b>" + tr("Connection failed!") + "</b>";
    _ret << "<blockquote>" + _db.lastError().text() + "</blockquote>";
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _user << _pass << _db.connectOptions();
#endif
  }
  // cleanup Connection
  _db.close();
  // push Message
  QMessageBox::information(this, tr("Connection test"), _ret.join("<br>"));
}

void ConfigDatabase::updateProfile() {
  const QString _id = m_profil->currentIdentifier();
  if (_id.isEmpty())
    return;

  if (resetInput())
    m_profil->setTestFocus();

  loadProfile(_id);
  setWindowModified(true);
}

void ConfigDatabase::loadSectionConfig() {
  m_profil->loadEntries();
  loadProfile(m_profil->currentIdentifier());
  registerInputChangeSignals(this);
}

void ConfigDatabase::saveSectionConfig() {
  const QString _profile_id = m_profil->currentIdentifier();
  config->setValue("database_profile", _profile_id);
  config->beginGroup("database/" + _profile_id);
  config->setValue("profile", m_profil->currentProfile());
  // SSL/TLS is always enabled!
  config->setValue("pg_ssl", true);
  QList<AntiquaCRM::AInputWidget *> _l =
      findChildren<AntiquaCRM::AInputWidget *>(QString());
  for (int i = 0; i < _l.count(); i++) {
    AntiquaCRM::AInputWidget *m_inp = _l.at(i);
    if (m_inp != nullptr) {
      const QString _key = m_inp->objectName();
      QVariant _value = m_inp->getValue();
#ifdef ANTIQUA_DEVELOPEMENT
      if (_key.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Missing objectName!" << _value;
        continue;
      }
#endif
      if (!_value.isValid())
        config->remove(_key);
      else
        config->setValue(_key, _value);
    }
  }
  config->setValue("ssl_bundle", ssl_CA->getBundle());
  config->endGroup();
  setWindowModified(false);
}

AntiquaCRM::ConfigType ConfigDatabase::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigDatabase::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "network-server-database");
  _o.insert("title", tr("Database"));
  _o.insert("tooltip", tr("Database configuration."));
  return _o;
}
