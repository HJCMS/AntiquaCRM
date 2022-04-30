#include "postgresqlsettings.h"
#include "filedialog.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

PostgreSqlSettings::PostgreSqlSettings(QWidget *parent)
    : SettingsWidget{parent} {
  setObjectName("sql_config_widget");
  setWindowTitle(tr("Database Server Configuration"));
  setConfigSection("postgresql");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("sql_config_layout");

  QHBoxLayout *titleLayout = new QHBoxLayout();
  QLabel *lt_m = new QLabel(this);
  lt_m->setObjectName("pagetitel");
  lt_m->setText(tr("Connection settings to PostgreSQL server."));
  titleLayout->addWidget(lt_m);

  QSpacerItem *sp =
      new QSpacerItem(10, 10, QSizePolicy::Maximum, QSizePolicy::Minimum);
  titleLayout->addSpacerItem(sp);

  mainLayout->addLayout(titleLayout);

  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setObjectName("gridLayout");

  QLabel *lt_hostname = new QLabel(this);
  lt_hostname->setObjectName("lb2");
  lt_hostname->setAlignment(Qt::AlignRight | Qt::AlignTrailing |
                            Qt::AlignVCenter);
  lt_hostname->setText(tr("Hostname"));
  gridLayout->addWidget(lt_hostname, 0, 0, 1, 1);

  psql_hostname = new QLineEdit(this);
  psql_hostname->setObjectName("psql_hostname");
  psql_hostname->setToolTip(
      tr("In your Network a DNS Server exists. Then use a Fully Qualified "
         "Hostname (FQDN) to connect the PostgreSQL Server. If Not, it's "
         "better to insert the IP Address in here. Warning: SSL Certification "
         "based Connection need a Hostname for translate the <b>CommonName</b> "
         "Attribute."));
  gridLayout->addWidget(psql_hostname, 0, 1, 1, 1);

  QLabel *lt_port = new QLabel(this);
  lt_port->setObjectName("lb3");
  lt_port->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
  lt_port->setText(tr("Port Address"));
  gridLayout->addWidget(lt_port, 1, 0, 1, 1);

  QHBoxLayout *m_portLayout = new QHBoxLayout();
  m_portLayout->setObjectName("portLayout");

  psql_port = new QSpinBox(this);
  psql_port->setObjectName("psql_port");
  psql_port->setMinimum(1024);
  psql_port->setMaximum(9999);
  psql_port->setValue(5432);
  psql_port->setToolTip(tr("The default port address is 5432"));
  m_portLayout->addWidget(psql_port);

  QSpacerItem *horizontalSpacer =
      new QSpacerItem(100, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  m_portLayout->addItem(horizontalSpacer);
  gridLayout->addLayout(m_portLayout, 1, 1, 1, 1);

  QLabel *lt_dbname = new QLabel(this);
  lt_dbname->setObjectName("lb4");
  lt_dbname->setAlignment(Qt::AlignRight | Qt::AlignTrailing |
                          Qt::AlignVCenter);
  lt_dbname->setText(tr("Databasename"));
  gridLayout->addWidget(lt_dbname, 2, 0, 1, 1);

  psql_databasename = new QLineEdit(this);
  psql_databasename->setObjectName("psql_databasename");
  psql_databasename->setToolTip(tr("The name must be letters with no spaces."));
  gridLayout->addWidget(psql_databasename, 2, 1, 1, 1);

  QLabel *lt_dbuser = new QLabel(this);
  lt_dbuser->setObjectName("lb4");
  lt_dbuser->setAlignment(Qt::AlignRight | Qt::AlignTrailing |
                          Qt::AlignVCenter);
  lt_dbuser->setText(tr("Loginname"));
  gridLayout->addWidget(lt_dbuser, 3, 0, 1, 1);

  psql_username = new QLineEdit(this);
  psql_username->setObjectName("psql_username");
  psql_username->setToolTip(tr("The username must be letters with no spaces."));
  gridLayout->addWidget(psql_username, 3, 1, 1, 1);

  QLabel *lt_password = new QLabel(this);
  lt_password->setObjectName("lb5");
  lt_password->setAlignment(Qt::AlignRight | Qt::AlignTrailing |
                            Qt::AlignVCenter);
  lt_password->setText(tr("Password"));
  gridLayout->addWidget(lt_password, 4, 0, 1, 1);

  psql_password = new QLineEdit(this);
  psql_password->setObjectName("psql_password");
  psql_password->setInputMethodHints(Qt::ImhHiddenText);
  psql_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  gridLayout->addWidget(psql_password, 4, 1, 1, 1);

  mainLayout->addLayout(gridLayout);

  psql_ssl = new QGroupBox(this);
  psql_ssl->setObjectName("sslGroupBox");
  psql_ssl->setMinimumSize(QSize(100, 150));
  psql_ssl->setCheckable(true);
  psql_ssl->setChecked(false);
  psql_ssl->setTitle(tr("Enable SSL Connection"));

  QVBoxLayout *verticalLayout = new QVBoxLayout(psql_ssl);
  verticalLayout->setObjectName("verticalLayout");

  QHBoxLayout *m_hL2 = new QHBoxLayout();
  m_hL2->setObjectName("m_hL2");

  QLabel *lb6 = new QLabel(psql_ssl);
  lb6->setObjectName("lb6");
  lb6->setText(tr("Clientcert"));
  m_hL2->addWidget(lb6);

  psql_ssl_clientcert = new QLineEdit(psql_ssl);
  psql_ssl_clientcert->setObjectName("psql_ssl_clientcert");
  m_hL2->addWidget(psql_ssl_clientcert);

  QToolButton *psql_open_cert = new QToolButton(this);
  psql_open_cert->setObjectName("open_cert_button");
  psql_open_cert->setIcon(myIcon("folder_green"));
  m_hL2->addWidget(psql_open_cert);

  verticalLayout->addLayout(m_hL2);

  QLabel *lb7 = new QLabel(psql_ssl);
  lb7->setObjectName("lb7");
  lb7->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
  lb7->setWordWrap(true);
  lb7->setText(
      tr("If you using SSL/TLS or SSL Client certificates. Make sure, you have "
         "also the CA-Root Certificate in your System Issuer cache."));
  verticalLayout->addWidget(lb7);

  mainLayout->addWidget(psql_ssl);

  QSpacerItem *vsp =
      new QSpacerItem(50, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
  mainLayout->addItem(vsp);

  setLayout(mainLayout);

  connect(psql_open_cert, SIGNAL(clicked()), this, SLOT(openCertFileDialog()));
}

void PostgreSqlSettings::openCertFileDialog() {
  FileDialog *m_d = new FileDialog(this);
  m_d->setObjectName("certfile_dialog");
  m_d->setCertsFilter();
  if (m_d->exec()) {
    QStringList list = m_d->selectedFiles();
    psql_ssl_clientcert->setText(list.first());
  }
}

void PostgreSqlSettings::updateConfigSets(const QHash<QString, QVariant> &d) {
  if (d.isEmpty())
    return;

  QByteArray pw = QByteArray::fromBase64(d.value("password").toByteArray(),
                                         QByteArray::Base64UrlEncoding);
  QString pass(pw);

  psql_hostname->setText(d.value("hostname").toString());
  psql_databasename->setText(d.value("database").toString());
  psql_username->setText(d.value("username").toString());
  psql_password->setText(pass);
  psql_port->setValue(d.value("port").toInt());
  psql_ssl->setChecked(d.value("ssl").toBool());
  psql_ssl_clientcert->setText(d.value("certpath").toString());
}

const QHash<QString, QVariant> &PostgreSqlSettings::getSectionConfig() {
  if (!p_hash.isEmpty())
    p_hash.clear();

  p_hash.insert("hostname", psql_hostname->text());
  p_hash.insert("database", psql_databasename->text());
  p_hash.insert("username", psql_username->text());
  QByteArray pw =
      psql_password->text().toLocal8Bit().toBase64(QByteArray::Base64Encoding);
  p_hash.insert("password", pw);
  p_hash.insert("port", psql_port->value());
  p_hash.insert("ssl", psql_ssl->isChecked());
  p_hash.insert("certpath", psql_ssl_clientcert->text());
  return p_hash;
}
