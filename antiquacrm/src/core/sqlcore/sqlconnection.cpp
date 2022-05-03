// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlconnection.h"
#include "sqlconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpSocket>

namespace HJCMS {

SqlConnection::SqlConnection(QObject *parent) : QObject{parent} {
  setObjectName("SqlConnection");
  config = new SqlConfig(this);
}

const QString SqlConnection::peerCertfile() { return config->getCertificate(); }

const QString SqlConnection::peerKeyfile() { return config->getPrivateKey(); }

const QString SqlConnection::rootCaFile() { return config->getCaRootCert(); }

void SqlConnection::isEncrypted() { qDebug() << Q_FUNC_INFO; }

bool SqlConnection::simpleConnect() {
  bool b = true;
  int t = (timeout * 1000);
  QTcpSocket *m_tcp = new QTcpSocket(this);
  m_tcp->connectToHost(config->getAddress(), config->getPort(),
                       QIODevice::ReadOnly);
  if (!m_tcp->waitForConnected(t)) {
    emit warnMessage(tr("No Database server connected!"));
    b = false;
  }
  m_tcp->close();
  return b;
}

bool SqlConnection::secureConnect() {
  bool b = false;
  int t = (timeout * 1000);

  QSslConfiguration sslConfig;
  sslConfig.addCaCertificate(config->getCaCert());

  QSslSocket *m_ssl = new QSslSocket(this);
  m_ssl->setProtocol(QSsl::DtlsV1_2OrLater);
  m_ssl->setSslConfiguration(sslConfig);

  QObject::connect(m_ssl, SIGNAL(encrypted()), this, SLOT(isEncrypted()));
  m_ssl->connectToHostEncrypted(config->getAddress(), config->getPort());
  if (m_ssl->waitForConnected(t)) {
    b = true;
  }
  m_ssl->close();
  if (!b) {
    emit warnMessage(tr("No Secured Database server connected!"));
  }
  return b;
}

bool SqlConnection::connect(int t) {
  timeout = t;
  return simpleConnect();
}

SqlConnection::~SqlConnection() {}

}; // namespace HJCMS
