// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlconnection.h"
#include "sqlconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QSslConfiguration>

namespace HJCMS {

static QSslConfiguration sslConfig()
{
  HJCMS::SqlConfig config;
  QSslConfiguration ssl;
  ssl.addCaCertificates(config.getCertificate());
  return ssl;
}

SqlConnection::SqlConnection(QObject *parent) : QObject{parent} {
  setObjectName("SqlConnection");
  config = new SqlConfig(this);
}

const QSslCertificate SqlConnection::peerCert() {
  QString p = config->getCertificate();
  if (p.isEmpty())
    return QSslCertificate();

  QByteArray buffer;
  QFile fp(p);
  if (fp.open(QIODevice::ReadOnly)) {
    while (!fp.atEnd()) {
      buffer.append(fp.readLine());
    }
    fp.close();
  } else {
    return QSslCertificate();
  }

  QSslCertificate sslCert(buffer, QSsl::Pem);
  return sslCert;
}

void SqlConnection::isEncrypted()
{
  qDebug() << Q_FUNC_INFO;
}

bool SqlConnection::simpleConnect() {
  bool b = true;
  int t = (timeout * 1000);
  QTcpSocket *m_tcp = new QTcpSocket(this);
  m_tcp->connectToHost(config->getAddress(),config->getPort(),QIODevice::ReadOnly);
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
  QSslSocket *m_ssl = new QSslSocket(this);
  m_ssl->setProtocol(QSsl::DtlsV1_2OrLater);
  m_ssl->setSslConfiguration(sslConfig());
  // m_ssl->setLocalCertificate(peerCert());
  QObject::connect(m_ssl, SIGNAL(encrypted()), this, SLOT(isEncrypted()));
  m_ssl->connectToHostEncrypted(config->getAddress(), config->getPort());
  if (m_ssl->waitForConnected(t)) {
    qDebug() << Q_FUNC_INFO << "TODO" << m_ssl->isEncrypted();
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
  if (config->isSecureEnabled()) {
    return secureConnect();
  } else {
    return simpleConnect();
  }
  return false;
}

SqlConnection::~SqlConnection() {}

}; // namespace HJCMS
