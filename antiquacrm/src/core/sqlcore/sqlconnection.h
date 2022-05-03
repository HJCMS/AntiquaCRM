// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCONNECTION_SQLCORE_H
#define HJCMS_SQLCONNECTION_SQLCORE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QSslCertificate>

namespace HJCMS {

class SqlConfig;

class SqlConnection : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int timeout = 2; /**< Sekunden bis abbruch */
  SqlConfig *config;

private:
  const QString peerCertfile();
  const QString peerKeyfile();
  const QString rootCaFile();

  /**
   * @brief TCPSocket Verbindung
   */
  bool simpleConnect();

  /**
   * @brief SSLSocket Verbindung
   */
  bool secureConnect();

private Q_SLOTS:
  void isEncrypted();

Q_SIGNALS:
  /**
   * @brief Warnmeldungen
   */
  void warnMessage(const QString &);

public:
  explicit SqlConnection(QObject *parent = nullptr);

  /**
   * @brief Verbindung aufbauen:
   * @param t Sekunden
   * @return Gibt bei Erfolg true zurück.
   */
  bool connect(int t = 2);
  virtual ~SqlConnection();
};

}; // namespace HJCMS

#endif // HJCMS_SQLCONNECTION_SQLCORE_H
