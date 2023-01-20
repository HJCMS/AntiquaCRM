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
#include <QPair>

class ConfigSSL : public QWidget {
  Q_OBJECT

private:
  QComboBox *ssl_mode;
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
  void setData(const QPair<QString,QVariant> &);
  const QMap<QString,QString> configuration();
  const QSslCertificate getCA();
};

#endif // CONFIGSSL_H
