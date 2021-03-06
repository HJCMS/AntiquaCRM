// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pgsqlsettings.h"
#include "filedialog.h"
#include "myicontheme.h"

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QGroupBox>
#include <QHash>
#include <QLineEdit>
#include <QSpinBox>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QStandardPaths>
#include <QToolButton>
#include <QWidget>
#include <QtWidgets>

PgSQLSettings::PgSQLSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("pgsql_settings");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("postgresql_config_layout");

  QString sql_hostname_info =
      tr("Hostname of the server you want to connect to. If you are not using "
         "SSL, the IP address can also be added in this field.");

  QString sql_database_info =
      tr("The name of the server database to which you want to connect.");

  QString sql_username_info =
      tr("The Username from the server database to which you want to connect.");

  QString sql_port_info =
      tr("Port number to connect to the server. The default port address to a "
         "PostgreSQL server is „5432“. However, if you connect from outside, "
         "you need the configured router port. Your administrator can "
         "communicate this.");

  QString ca_info = tr(
      "CA Bundle is the file that contains root and intermediate certificates. "
      "Together with your server certificate (issued specifically for your "
      "domain), these files complete the SSL chain of trust. The chain is "
      "required to verify the Authentication with the main application.");

  QString cn_info = tr(
      "The Common Name (CN), also known as the Fully Qualified Domain Name "
      "(FQDN). CN is the machine name of the remote system you want to "
      "connect. If the certificate CN is not the same on the connecting "
      "machine, the trust relationship between client and server will fail.");

  QString peer_info =
      tr("The peer authentication method works by obtaining the client's "
         "operating system user name from the system and using it as the "
         "allowed database user name. This method is only supported on "
         "local-system connections.");

  QString rootcert_info =
      tr("The certificate of the trusted certificate authority and not the "
         "server certificate! The server sends its certificate, which is "
         "checked by the issuer and classified as trustworthy.");

  QString sslmode_info =
      tr("For a connection to be known as SSL-secured, SSL usage must be "
         "configured on both the client and server before the connection is "
         "established. If it is only configured on the server, the client may "
         "send sensitive information (e.g. passwords) before knowing that the "
         "server requires high security. In this case, secure connections can "
         "be guaranteed by setting the SSL connection to „Required“, „Verify "
         "full“ or „Verify-CA“.");

  QString ssl_pgsql_link_description("");
  ssl_pgsql_link_description.append(
      tr("Full Information about Secured SQL Connections, you can find at"));
  ssl_pgsql_link_description.append(" <a href='");
  ssl_pgsql_link_description.append(pgsqlClientAuthDocUrl().toString());
  ssl_pgsql_link_description.append("' target='_blank'>");
  ssl_pgsql_link_description.append(pgsqlClientAuthDocUrl().toString());
  ssl_pgsql_link_description.append("</a>");

  QString title = tr("Database Connection settings to PostgreSQL server.");
  QLabel *lb_title = new QLabel(this);
  lb_title->setText(title);
  layout->addWidget(lb_title);

  QGroupBox *group_connect = new QGroupBox(this);
  group_connect->setTitle(tr("Access Configuration"));
  QVBoxLayout *g_sql_layout = new QVBoxLayout(group_connect);

  sql_hostname = new LineEdit(this);
  sql_hostname->setObjectName("hostname");
  sql_hostname->setInfo(tr("Hostname/Address"));
  sql_hostname->setPlaceholderText(tr("192.168.178.2"));
  sql_hostname->setRequired(true);
  sql_hostname->setWhatsThis(sql_hostname_info);
  g_sql_layout->addWidget(sql_hostname);

  sql_databasename = new LineEdit(this);
  sql_databasename->setObjectName("database");
  sql_databasename->setInfo(tr("Databasename"));
  sql_databasename->setPlaceholderText(tr("antiquacrm"));
  sql_databasename->setRequired(true);
  sql_databasename->setWhatsThis(sql_database_info);
  g_sql_layout->addWidget(sql_databasename);

  sql_username = new LineEdit(this);
  sql_username->setObjectName("username");
  sql_username->setInfo(tr("Database Username"));
  sql_username->setPlaceholderText(tr("antiquacrm"));
  sql_username->setRequired(true);
  sql_username->setWhatsThis(sql_username_info);
  g_sql_layout->addWidget(sql_username);

  sql_password = new LineEdit(this);
  sql_password->setObjectName("password");
  sql_password->setInfo(tr("Database Password"));
  sql_password->setPlaceholderText(tr("min length 8"));
  sql_password->setRequired(true);
  sql_password->setPasswordInput(true);
  g_sql_layout->addWidget(sql_password);

  QHBoxLayout *gh_sql_alyout = new QHBoxLayout();
  gh_sql_alyout->addStretch(1);

  sql_port = new IntSpinBox(1024, 9999, this);
  sql_port->setObjectName("port");
  sql_port->setValue(5432);
  sql_port->setToolTip(tr("The default port address is 5432"));
  sql_port->setInfo(tr("Port"));
  sql_port->setRequired(true);
  sql_port->setWhatsThis(sql_port_info);
  gh_sql_alyout->addWidget(sql_port);

  sql_timeout = new IntSpinBox(5, 30, this);
  sql_timeout->setObjectName("connect_timeout");
  sql_timeout->setValue(5);
  sql_timeout->setInfo(tr("Timeout"));
  sql_timeout->setToolTip(tr("Timeout in seconds"));
  gh_sql_alyout->addWidget(sql_timeout);

  sql_ssl = new BoolBox(this);
  sql_ssl->setObjectName("ssl");
  sql_ssl->setChecked(false);
  sql_ssl->setInfo(tr("Enable SSL/TLS"));
  sql_ssl->setToolTip(tr("Enable SSL/TLS secure Connection configuration."));
  gh_sql_alyout->addWidget(sql_ssl);

  g_sql_layout->addLayout(gh_sql_alyout);
  group_connect->setLayout(g_sql_layout);
  layout->addWidget(group_connect);

  // BEGIN SSL/TLS
  // https://www.postgresql.org/docs/current/libpq-ssl.html
  // https://www.postgresql.org/docs/current/ssl-tcp.html
  m_tls = new QGroupBox(this);
  m_tls->setEnabled(false);
  m_tls->setTitle(tr("SSL/TLS Connection"));
  QGridLayout *tls_layout = new QGridLayout(m_tls);

  ssl_ca_CN = new QComboBox(m_tls);
  ssl_ca_CN->setObjectName("ssl_CA");
  ssl_ca_CN->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  ssl_ca_CN->setWhatsThis(ca_info);
  tls_layout->addWidget(ssl_ca_CN, 0, 0, 1, 1);

  QPushButton *btn_ca_bundle = new QPushButton(m_tls);
  btn_ca_bundle->setText(tr("CA Bundle"));
  btn_ca_bundle->setIcon(myIcon("folder_red"));
  btn_ca_bundle->setToolTip(tr("Open ca-bundle.* in Pem Format"));
  btn_ca_bundle->setWhatsThis(ca_info);
  tls_layout->addWidget(btn_ca_bundle, 0, 1, 1, 1);

  ssl_ca_bundle = new LineEdit(m_tls);
  ssl_ca_bundle->setObjectName("ssl_bundle");
  ssl_ca_bundle->setInfo(tr("CA Bundle"));
  ssl_ca_bundle->setWhatsThis(ca_info);
  tls_layout->addWidget(ssl_ca_bundle, 1, 0, 1, 2);

  ssl_CN = new LineEdit(m_tls);
  ssl_CN->setObjectName("ssl_CN");
  ssl_CN->setInfo(tr("Servercertificate (CN)"));
  ssl_CN->setWhatsThis(cn_info);
  tls_layout->addWidget(ssl_CN, 2, 0, 1, 2);

  /**
   * Trusted Certificate Authorities
   * Checks that server certificate is signed by a trusted certificate authority
   */
  ssl_root_cert = new LineEdit(m_tls);
  ssl_root_cert->setObjectName("ssl_root_cert");
  ssl_root_cert->setInfo(tr("Exhibitor"));
  ssl_root_cert->setToolTip("Exhibitor Certificate");
  ssl_root_cert->setPlaceholderText("Exhibitor Certification path");
  ssl_root_cert->setWhatsThis(rootcert_info);
  tls_layout->addWidget(ssl_root_cert, 3, 0, 1, 1);

  QPushButton *btn_root_cert = new QPushButton(m_tls);
  btn_root_cert->setText(tr("Exhibitor Certificate"));
  btn_root_cert->setIcon(myIcon("folder_red"));
  btn_root_cert->setToolTip(tr("Open Server Exhibitor Certificate"));
  tls_layout->addWidget(btn_root_cert, 3, 1, 1, 1);

  QLabel *modeinfo = new QLabel(m_tls);
  modeinfo->setAlignment(grid_label_align);
  modeinfo->setText(tr("SSL/TLS Verification priority"));
  tls_layout->addWidget(modeinfo, 4, 0, 1, 1);

  ssl_mode = new QComboBox(m_tls);
  ssl_mode->setObjectName("ssl_mode");
  ssl_mode->setToolTip(tr("SSL Protection Provided in Different Modes."));
  ssl_mode->setWhatsThis(sslmode_info);
  ssl_mode->insertItem(0, tr("Prefer"), QVariant("prefer"));
  ssl_mode->insertItem(1, tr("Required"), QVariant("require"));
  ssl_mode->insertItem(2, tr("Verify CA"), QVariant("verify-ca"));
  ssl_mode->insertItem(3, tr("Verify full"), QVariant("verify-full"));
  tls_layout->addWidget(ssl_mode, 4, 1, 1, 1);

  m_tls->setLayout(tls_layout);
  layout->addWidget(m_tls);
  // END SSL/TLS

  // BEGIN SSL Peer
  // https://www.postgresql.org/docs/current/libpq-ssl.html
  ssl_peer = new QGroupBox(this);
  ssl_peer->setCheckable(true);
  ssl_peer->setChecked(false);
  ssl_peer->setTitle(tr("Local SSL Peer Connections"));
  ssl_peer->setWhatsThis(peer_info);
  QVBoxLayout *peer_layout = new QVBoxLayout(ssl_peer);

  ssl_peer_cert = new LineEdit(ssl_peer);
  ssl_peer_cert->setObjectName("ssl_peer_cert");
  ssl_peer_cert->setInfo(tr("Certificate"));
  peer_layout->addWidget(ssl_peer_cert);

  ssl_peer_key = new LineEdit(ssl_peer);
  ssl_peer_key->setObjectName("ssl_peer_key");
  ssl_peer_key->setInfo(tr("Private Keyfile"));
  peer_layout->addWidget(ssl_peer_key);

  ssl_peer_pass = new LineEdit(ssl_peer);
  ssl_peer_pass->setObjectName("ssl_peer_pass");
  ssl_peer_pass->setInfo(tr("Password"));
  peer_layout->addWidget(ssl_peer_pass);

  ssl_peer->setLayout(peer_layout);
  layout->addWidget(ssl_peer);
  // END SSL Peer

  QLabel *infolink = new QLabel(this);
  infolink->setWordWrap(true);
  infolink->setOpenExternalLinks(true);
  infolink->setText(ssl_pgsql_link_description);
  layout->addWidget(infolink);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_ca_bundle, SIGNAL(clicked()), this, SLOT(openCaBundle()));
  connect(btn_root_cert, SIGNAL(clicked()), this, SLOT(openRootCert()));
  connect(sql_ssl, SIGNAL(checked(bool)), m_tls, SLOT(setEnabled(bool)));
  connect(m_tls, SIGNAL(clicked(bool)), this, SLOT(chieldModified(bool)));
  connect(ssl_mode, SIGNAL(currentIndexChanged(int)), this,
          SLOT(chieldChanged(int)));
  connect(ssl_ca_CN, SIGNAL(currentIndexChanged(int)), this,
          SLOT(chieldChanged(int)));
  connect(ssl_peer, SIGNAL(clicked(bool)), this, SLOT(chieldModified(bool)));
}

void PgSQLSettings::initSignalChanged() {
  QList<UtilsMain *> l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(hasModified(bool)), this, SLOT(chieldModified(bool)));
      }
    }
  }
}

void PgSQLSettings::initCaBundleData(const QString &bundle) {
  if (bundle.isEmpty())
    return;

  QSslConfiguration sslConfig;
  if (sslConfig.addCaCertificates(bundle, QSsl::Pem)) {
    ssl_ca_CN->clear();
    QList<QSslCertificate> list = sslConfig.caCertificates();
    for (int i = 0; i < list.size(); i++) {
      QSslCertificate cert = list.at(i);
      if (!cert.isNull()) {
        ssl_ca_CN->addItem(cert.issuerDisplayName());
      }
    }
  }
}

const QString PgSQLSettings::openFileDialog(const QString &dest) {
  QString dir = (dest.isEmpty()) ? QDir::homePath() : dest;
  QString title = tr("Open Certfile");
  QString type = tr("Certificates") + " (*.pem *.crt *.cert *.key)";
  QString cert = QFileDialog::getOpenFileName(this, title, dir, type);
  return (cert.isEmpty()) ? QString() : cert;
}

void PgSQLSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString PgSQLSettings::getPageTitle() { return pageTitle; }

void PgSQLSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon PgSQLSettings::getPageIcon() { return pageIcon; }

void PgSQLSettings::openCaBundle() {
  QString ca = openFileDialog(QDir::rootPath());
  if (ca.isEmpty())
    return;

  ssl_ca_bundle->setValue(ca);
  initCaBundleData(ca);
}

void PgSQLSettings::openRootCert() {
  QString cert = openFileDialog(QDir::homePath());
  if (cert.isEmpty())
    return;

  ssl_root_cert->setValue(cert);
}

void PgSQLSettings::loadSectionConfig() {
  QHash<QString, QVariant> psql = config->readGroupConfig("postgresql");
  sql_hostname->setValue(psql.value("hostname"));
  sql_databasename->setValue(psql.value("database"));
  sql_username->setValue(psql.value("username"));
  QByteArray pw = QByteArray::fromBase64(psql.value("password").toByteArray(),
                                         QByteArray::Base64UrlEncoding);
  sql_password->setValue(QString(pw));
  sql_port->setValue(psql.value("port"));
  sql_timeout->setValue(psql.value("connect_timeout"));
  sql_ssl->setValue(psql.value("ssl"));
  QHash<QString, QVariant> ssloptions = config->readGroupConfig("ssloptions");
  QString ca_bundle = ssloptions.value("ssl_bundle").toString();
  ssl_ca_bundle->setValue(ca_bundle);
  ssl_root_cert->setValue(ssloptions.value("ssl_root_cert"));
  ssl_CN->setValue(ssloptions.value("ssl_CN"));
  initCaBundleData(ca_bundle);
  ssl_peer->setChecked(ssloptions.value("ssl_peer").toBool());
  ssl_peer_cert->setValue(ssloptions.value("ssl_peer_cert"));
  ssl_peer_key->setValue(ssloptions.value("ssl_peer_key"));
  ssl_peer_pass->setValue(ssloptions.value("ssl_peer_pass"));
  if (ssl_ca_CN->count() > 2) {
    int ca_index = ssl_ca_CN->findData(ssloptions.value("ssl_CA"),
                                       Qt::DisplayRole, Qt::MatchExactly);
    if (ca_index >= 0)
      ssl_ca_CN->setCurrentIndex(ca_index);
  }
  QString mode = ssloptions.value("ssl_mode").toString();
  if (!mode.isEmpty()) {
    int mode_index = ssl_mode->findData(mode, Qt::UserRole, Qt::MatchExactly);
    if (mode_index >= 0)
      ssl_mode->setCurrentIndex(mode_index);
  }
  initSignalChanged();
}

void PgSQLSettings::saveSectionConfig() {
  QHash<QString, QVariant> pg_options;
  pg_options.insert("hostname", sql_hostname->value());
  pg_options.insert("database", sql_databasename->value());
  pg_options.insert("username", sql_username->value());
  QByteArray pw =
      sql_password->value().toByteArray().toBase64(QByteArray::Base64Encoding);
  pg_options.insert("password", pw);
  pg_options.insert("port", sql_port->value());
  pg_options.insert("timeout", sql_timeout->value());
  pg_options.insert("ssl", sql_ssl->value());
  config->writeGroupConfig("postgresql", pg_options);

  QHash<QString, QVariant> ssl_options;
  ssl_options.insert("ssl_CA", ssl_ca_CN->currentData(Qt::DisplayRole));
  ssl_options.insert("ssl_mode", ssl_mode->currentData(Qt::UserRole));
  ssl_options.insert("ssl_CN", ssl_CN->value());
  ssl_options.insert("ssl_bundle", ssl_ca_bundle->value());
  ssl_options.insert("ssl_peer", ssl_peer->isChecked());
  ssl_options.insert("ssl_peer_cert", ssl_peer_cert->value());
  ssl_options.insert("ssl_peer_key", ssl_peer_key->value());
  ssl_options.insert("ssl_peer_pass", ssl_peer_pass->value());
  ssl_options.insert("ssl_root_cert", ssl_root_cert->value());
  config->writeGroupConfig("ssloptions", ssl_options);
}
