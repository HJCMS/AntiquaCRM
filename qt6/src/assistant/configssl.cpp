// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configssl.h"

#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSslConfiguration>
#include <QMetaType>
#include <QVBoxLayout>

ConfigSSL::ConfigSSL(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *title = new QLabel(this);
  QString header("<h3>" + tr("SSL/TLS Configuration") + "</h3><p>");
  header.append(tr("if not know about this settings, don't touch it."));
  header.append("</p>");
  title->setText(header);
  layout->addWidget(title);

  QLabel *t_sslmode = new QLabel(this);
  t_sslmode->setText(tr("Server Certificate: CommonName"));
  layout->addWidget(t_sslmode);

  ssl_mode = new QComboBox(this);
  ssl_mode->setObjectName("ssl_mode");
  ssl_mode->setPlaceholderText("verify");
  ssl_mode->insertItem(0, "Prefer", QVariant("prefer"));
  ssl_mode->insertItem(1, "Required", QVariant("require"));
  ssl_mode->insertItem(2, "Verify CA", QVariant("verify-ca"));
  ssl_mode->insertItem(3, "Verify full", QVariant("verify-full"));
  layout->addWidget(ssl_mode);

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

  if (name == "ssl_mode") {
    ssl_mode->setCurrentIndex(ssl_mode->findData(value));
    return;
  }

  QLineEdit *e = findChild<QLineEdit *>(name);
  if ((e != nullptr) &&  (value.metaType().id() == QMetaType::QString)) {
    QString str = value.toString().trimmed();
    e->setText(str);
    if (name == "ssl_bundle")
      initCACertificates(str);
  }
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
      this, tr("Open CA Bundle"), QDir::rootPath(),
      tr("Certificate (*.pem *.ca *.crt *.cert *.key)"));
  if (!path.isEmpty()) {
    sslCaBundlePath->setText(path);
    initCACertificates(path);
  }
}

const QMap<QString, QString> ConfigSSL::configuration() {
  QMap<QString, QString> m;
  QString mode =
      ssl_mode->itemData(ssl_mode->currentIndex(), Qt::UserRole).toString();
  m.insert("ssl_mode", mode);
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
