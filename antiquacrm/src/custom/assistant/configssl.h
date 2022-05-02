// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CONFIGSSL_H
#define CONFIGSSL_H

#include <QGroupBox>
#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QSslCertificate>
#include <QMap>

class ConfigSSL : public QWidget {
  Q_OBJECT

private:
  QGroupBox *sslPeer;
  QLineEdit *sslPeerCertPath;
  QLineEdit *sslKeyFilePath;
  QLineEdit *sslCommonName;
  QLineEdit *sslCaBundlePath;
  QLineEdit *sslCaIssuer;
  QComboBox *sslIssuers;

  void initCACertificates(const QString &);

private Q_SLOTS:
  void setCaChanged(int);
  void setCaBundlePath();

public:
  explicit ConfigSSL(QWidget *parent = nullptr);
  const QMap<QString,QString> configuration();
  const QSslCertificate getCA();
};

#endif // CONFIGSSL_H
