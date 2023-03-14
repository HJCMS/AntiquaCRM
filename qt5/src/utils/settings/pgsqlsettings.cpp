// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pgsqlsettings.h"

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

  QString pg_hostname_info =
      tr("Hostname of the server you want to connect to. If you are not using "
         "SSL, the IP address can also be added in this field.");

  QString sql_database_info =
      tr("The name of the server database to which you want to connect.");

  QString pg_username_info =
      tr("The Username from the server database to which you want to connect.");

  QString pg_port_info =
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

  sql_profiles = new QComboBox(this);
  sql_profiles->setObjectName("database_profile");
  sql_profiles->setToolTip(tr("Database profile"));
  layout->addWidget(sql_profiles);

  QGroupBox *group_connect = new QGroupBox(this);
  group_connect->setTitle(tr("Access Configuration"));
  QVBoxLayout *g_sql_layout = new QVBoxLayout(group_connect);

  pg_hostname = new LineEdit(this);
  pg_hostname->setObjectName("pg_hostname");
  pg_hostname->setInfo(tr("Hostname/Address"));
  pg_hostname->setRequired(true);
  pg_hostname->setWhatsThis(pg_hostname_info);
  g_sql_layout->addWidget(pg_hostname);

  pg_database = new LineEdit(this);
  pg_database->setObjectName("pg_database");
  pg_database->setInfo(tr("Databasename"));
  pg_database->setRequired(true);
  pg_database->setWhatsThis(sql_database_info);
  g_sql_layout->addWidget(pg_database);

  pg_username = new LineEdit(this);
  pg_username->setObjectName("pg_username");
  pg_username->setInfo(tr("Database Username"));
  pg_username->setRequired(true);
  pg_username->setWhatsThis(pg_username_info);
  g_sql_layout->addWidget(pg_username);

  pg_password = new LineEdit(this);
  pg_password->setObjectName("pg_password");
  pg_password->setInfo(tr("Database Password"));
  pg_password->setRequired(true);
  pg_password->setPasswordInput(true);
  g_sql_layout->addWidget(pg_password);

  QHBoxLayout *gh_sql_alyout = new QHBoxLayout();
  gh_sql_alyout->addStretch(1);

  pg_port = new IntSpinBox(1110, 45000, this);
  pg_port->setObjectName("pg_port");
  pg_port->setValue(QVariant(5432));
  pg_port->setToolTip(tr("The default port address is 5432"));
  pg_port->setInfo(tr("Port"));
  pg_port->setRequired(true);
  pg_port->setWhatsThis(pg_port_info);
  gh_sql_alyout->addWidget(pg_port);

  pg_timeout = new IntSpinBox(5, 30, this);
  pg_timeout->setObjectName("pg_timeout");
  pg_timeout->setValue(QVariant(5));
  pg_timeout->setInfo(tr("Timeout"));
  pg_timeout->setToolTip(tr("Timeout in seconds"));
  gh_sql_alyout->addWidget(pg_timeout);

  pg_ssl = new BoolBox(this);
  pg_ssl->setObjectName("pg_ssl");
  pg_ssl->setChecked(false);
  pg_ssl->setInfo(tr("Enable SSL/TLS"));
  pg_ssl->setToolTip(tr("Enable SSL/TLS secure Connection configuration."));
  gh_sql_alyout->addWidget(pg_ssl);

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

  ssl_CA = new QComboBox(m_tls);
  ssl_CA->setObjectName("ssl_CA");
  ssl_CA->setWhatsThis(ca_info);
  tls_layout->addWidget(ssl_CA, 0, 0, 1, 1);

  QPushButton *btn_ca_bundle = new QPushButton(m_tls);
  btn_ca_bundle->setText(tr("CA Bundle"));
  btn_ca_bundle->setIcon(getIcon("folder_red"));
  btn_ca_bundle->setToolTip(tr("Open ca-bundle.* in Pem Format"));
  btn_ca_bundle->setWhatsThis(ca_info);
  tls_layout->addWidget(btn_ca_bundle, 0, 1, 1, 1);

  ssl_bundle = new LineEdit(m_tls);
  ssl_bundle->setObjectName("ssl_bundle");
  ssl_bundle->setInfo(tr("CA Bundle"));
  ssl_bundle->setWhatsThis(ca_info);
  tls_layout->addWidget(ssl_bundle, 1, 0, 1, 2);

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
  ssl_root_cert->setWhatsThis(rootcert_info);
  tls_layout->addWidget(ssl_root_cert, 3, 0, 1, 1);

  QPushButton *btn_root_cert = new QPushButton(m_tls);
  btn_root_cert->setText(tr("Exhibitor Certificate"));
  btn_root_cert->setIcon(getIcon("folder_red"));
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

  QLabel *infolink = new QLabel(this);
  infolink->setWordWrap(true);
  infolink->setOpenExternalLinks(true);
  infolink->setText(ssl_pgsql_link_description);
  layout->addWidget(infolink);

  layout->addStretch(1);
  setLayout(layout);

  connect(sql_profiles, SIGNAL(currentTextChanged(const QString &)),
          SLOT(openProfile(const QString &)));
  connect(btn_ca_bundle, SIGNAL(clicked()), SLOT(openCaBundle()));
  connect(btn_root_cert, SIGNAL(clicked()), SLOT(openRootCert()));
  connect(pg_ssl, SIGNAL(checked(bool)), m_tls, SLOT(setEnabled(bool)));
  connect(m_tls, SIGNAL(clicked(bool)), SLOT(chieldModified(bool)));
  connect(ssl_mode, SIGNAL(currentIndexChanged(int)), SLOT(chieldChanged(int)));
  connect(ssl_CA, SIGNAL(currentIndexChanged(int)), SLOT(chieldChanged(int)));
}

void PgSQLSettings::initSignalChanged() {
  QList<InputEdit *> l =
      findChildren<InputEdit *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      InputEdit *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(sendHasModified(bool)), this,
                SLOT(chieldModified(bool)));
      }
    }
  }
}

void PgSQLSettings::initCaBundleData(const QString &bundle) {
  if (bundle.isEmpty())
    return;

  QSslConfiguration sslConfig;
  if (sslConfig.addCaCertificates(bundle, QSsl::Pem)) {
    ssl_CA->clear();
    QList<QSslCertificate> list = sslConfig.caCertificates();
    for (int i = 0; i < list.size(); i++) {
      QSslCertificate cert = list.at(i);
      if (!cert.isNull()) {
        QString ca = cert.issuerDisplayName();
        ssl_CA->addItem(ca, ca);
      }
    }
  }
}

void PgSQLSettings::setComboBoxData(const QString &name,
                                    const QVariant &value) {
  QComboBox *m_c = findChild<QComboBox *>(name);
  if (m_c != nullptr) {
    int index = m_c->findData(value.toString(), Qt::UserRole, Qt::MatchExactly);
    if (index > 0)
      m_c->setCurrentIndex(index);
  }
}

const QVariant PgSQLSettings::getComboBoxData(const QString &name) {
  QComboBox *m_c = findChild<QComboBox *>(name);
  if (m_c != nullptr)
    return m_c->itemData(m_c->currentIndex(), Qt::UserRole);

  return QVariant();
}

const QString PgSQLSettings::openFileDialog(const QString &dest) {
  QString dir = (dest.isEmpty()) ? QDir::homePath() : dest;
  QString title = tr("Open Certfile");
  QString type = tr("Certificates") + " (*.pem *.ca *.crt *.cert *.key)";
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

  ssl_bundle->setValue(ca);
  initCaBundleData(ca);
}

void PgSQLSettings::openRootCert() {
  QString cert = openFileDialog(QDir::homePath());
  if (cert.isEmpty())
    return;

  ssl_root_cert->setValue(cert);
}

void PgSQLSettings::openProfile(const QString &name) {
  if (name.isEmpty())
    return;

  QHash<QString, QVariant> cf = config->readGroupSection("database", name);
  if (cf.size() < 1)
    return;

  QList<InputEdit *> iList = findChildren<InputEdit *>(pattern);
  for (int i = 0; i < iList.count(); i++) {
    InputEdit *m_i = iList.at(i);
    if (m_i != nullptr) {
      QString name = m_i->objectName();
      if (name.contains("pg_password")) {
        QByteArray pw = QByteArray::fromBase64(cf.value(name).toByteArray(),
                                               QByteArray::Base64UrlEncoding);
        m_i->setValue(pw);
      } else {
        m_i->setValue(cf.value(name));
      }
      m_i->setModified(false);
    }
  }

  initCaBundleData(ssl_bundle->value().toString());

  QList<QComboBox *> cList = findChildren<QComboBox *>(pattern);
  for (int i = 0; i < cList.count(); i++) {
    QComboBox *m_c = cList.at(i);
    if (m_c != nullptr) {
      QString name = m_c->objectName();
      setComboBoxData(name, cf.value(name));
    }
  }
}

void PgSQLSettings::loadSectionConfig() {
  QString pr = config->value("database_profile", "Default").toString();
  config->beginGroup("database");
  QStringList profiles = config->childGroups();
  sql_profiles->addItems(profiles);
  sql_profiles->setCurrentText(pr);
  config->endGroup();
  openProfile(pr);
  initSignalChanged();
}

void PgSQLSettings::saveSectionConfig() {
  QString profile = sql_profiles->currentText();
  config->setValue("database_profile", profile);
  config->beginGroup("database/" + profile);
  QList<InputEdit *> iList = findChildren<InputEdit *>(pattern);
  for (int i = 0; i < iList.count(); i++) {
    InputEdit *m_i = iList.at(i);
    if (m_i != nullptr) {
      QString name = m_i->objectName();
      QVariant value = m_i->value();
      if (name.contains("pg_password"))
        value = pg_password->value().toByteArray().toBase64(
            QByteArray::Base64Encoding);

      config->setValue(name, value);
      m_i->setModified(false);
    }
  }
  QList<QComboBox *> cList = findChildren<QComboBox *>(pattern);
  for (int i = 0; i < cList.count(); i++) {
    QComboBox *m_c = cList.at(i);
    if (m_c != nullptr) {
      QString name = m_c->objectName();
      QVariant value = getComboBoxData(name);
      config->setValue(name, value);
    }
  }
  config->endGroup();

  /**
   * Fixed Database Listener settings
   * - interval Timer trigger every second
   * - counter  Count down from 60 seconds
   */
  config->beginGroup("timer/dblistener");
  config->setValue("interval", 1);
  config->setValue("counter", 60);
  config->endGroup();
}
