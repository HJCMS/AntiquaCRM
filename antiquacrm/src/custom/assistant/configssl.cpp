// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configssl.h"

#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSslConfiguration>
#include <QVBoxLayout>

ConfigSSL::ConfigSSL(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *title = new QLabel(this);
  QString header("<h3>" + tr("SSL/TLS Configuration") + "</h3><p>");
  header.append(tr("if not know about this settings, don't touch it."));
  header.append("</p>");
  title->setText(header);
  layout->addWidget(title);

  sslPeer = new QGroupBox(this);
  sslPeer->setObjectName("ssl_peer");
  sslPeer->setCheckable(true);
  sslPeer->setTitle(tr("SSL/TLS Peer Configuration"));
  sslPeer->setChecked(false);
  QVBoxLayout *bl = new QVBoxLayout(sslPeer);
  sslPeerCertPath = new QLineEdit(sslPeer);
  sslPeerCertPath->setObjectName("ssl_peer_cert");
  sslPeerCertPath->setPlaceholderText(
      tr("Peer Certificate: ~/.postgresql/postgresql.crt"));
  bl->addWidget(sslPeerCertPath);
  sslKeyFilePath = new QLineEdit(sslPeer);
  sslKeyFilePath->setObjectName("ssl_peer_key");
  sslKeyFilePath->setPlaceholderText(
      tr("Private Keyfile: ~/.postgresql/postgresql.key"));
  bl->addWidget(sslKeyFilePath);
  sslPeerPass = new QLineEdit(sslPeer);
  sslPeerPass->setObjectName("ssl_peer_pass");
  sslPeerPass->setPlaceholderText(tr("Password"));
  sslPeerPass->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  bl->addWidget(sslPeerPass);
  sslPeer->setLayout(bl);
  layout->addWidget(sslPeer);

  QLabel *t_commonName = new QLabel(this);
  t_commonName->setText(tr("Server Certificate: CommonName"));
  layout->addWidget(t_commonName);

  sslCommonName = new QLineEdit(this);
  sslCommonName->setPlaceholderText("CN");
  sslCommonName->setObjectName("ssl_CN");
  layout->addWidget(sslCommonName);

  QLabel *t_issuerPath = new QLabel(this);
  t_issuerPath->setText(tr("Issuer (CA) from Server Certificate"));
  layout->addWidget(t_issuerPath);

  QHBoxLayout *sp_layout = new QHBoxLayout();
  sslCaBundlePath = new QLineEdit(this);
  sslCaBundlePath->setObjectName("ssl_bundle");
  sslCaBundlePath->setPlaceholderText(tr("CA Root directory"));
  sp_layout->addWidget(sslCaBundlePath);
  QPushButton *openFileDialog = new QPushButton(tr("Set"), this);
  sp_layout->addWidget(openFileDialog);
  layout->addLayout(sp_layout);

  sslIssuers = new QComboBox(this);
  layout->addWidget(sslIssuers);

  sslCaIssuer = new QLineEdit(this);
  sslCaIssuer->setObjectName("ssl_root_cert");
  sslCaIssuer->setPlaceholderText(tr("CA Issuer"));
  layout->addWidget(sslCaIssuer);

  layout->addStretch(1);
  setLayout(layout);

  connect(openFileDialog, SIGNAL(clicked()), this, SLOT(setCaBundlePath()));
  connect(sslIssuers, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setCaChanged(int)));
}

void ConfigSSL::setData(const QPair<QString, QVariant> &data) {
  QString name = data.first;
  QVariant value = data.second;
  if (name.isEmpty() || value.isNull())
    return;

  QLineEdit *e = findChild<QLineEdit *>(name);
  if (e != nullptr && value.type() == QVariant::String) {
    QString str = value.toString().trimmed();
    e->setText(str);
    if (name == "ssl_bundle")
      initCACertificates(str);
  }

  if (name == "ssl_peer")
    sslPeer->setChecked(value.toBool());
}

void ConfigSSL::initCACertificates(const QString &dest) {
  QSslConfiguration sslConfig;
  if (sslConfig.addCaCertificates(dest, QSsl::Pem)) {
    QList<QSslCertificate> list = sslConfig.caCertificates();
    for (int i = 0; i < list.size(); i++) {
      QSslCertificate cert = list.at(i);
      if (!cert.isNull()) {
        sslIssuers->addItem(cert.issuerDisplayName());
      }
    }
  }
}

void ConfigSSL::setCaChanged(int i) {
  QString ca = sslIssuers->itemData(i, Qt::DisplayRole).toString();
  if (!ca.isEmpty())
    sslCaIssuer->setText(ca);
}

void ConfigSSL::setCaBundlePath() {
  QString path = QFileDialog::getOpenFileName(
      this, tr("Open CA Bundle"), QDir::rootPath(), tr("Certificate (*.pem)"));
  if (!path.isEmpty()) {
    sslCaBundlePath->setText(path);
    initCACertificates(path);
  }
}

const QMap<QString, QString> ConfigSSL::configuration() {
  QMap<QString, QString> m;
  if (!sslPeerCertPath->text().isEmpty())
    m.insert("ssl_peer_cert", sslPeerCertPath->text());
  if (!sslKeyFilePath->text().isEmpty())
    m.insert("ssl_peer_keyfile", sslKeyFilePath->text());
  if (!sslCaBundlePath->text().isEmpty())
    m.insert("ssl_bundle", sslCaBundlePath->text());
  if (!sslCommonName->text().isEmpty())
    m.insert("ssl_CN", sslCommonName->text());
  if (!sslCaIssuer->text().isEmpty())
    m.insert("ssl_CA", sslCaIssuer->text());
  return m;
}

const QSslCertificate ConfigSSL::getCA() {
  QSslCertificate cert;
  if (sslCaBundlePath->text().isEmpty() || sslCaIssuer->text().isEmpty())
    return cert;

  QSslConfiguration sslConfig;
  QList<QSslCertificate> list = sslConfig.caCertificates();
  if (list.size() < 1)
    return cert;

  for (int i = 0; i < list.size(); i++) {
    QSslCertificate pem = list.at(i);
    if (!pem.isNull() && (pem.issuerDisplayName() == sslCaIssuer->text())) {
      return pem;
    }
  }
  return cert;
}
