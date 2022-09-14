// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <AntiquaCRM>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>

class SocketServer : public QLocalServer {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int timeout = 5000;

  /**
   * @brief Listener für eingehende Nachrichten
   */
  QLocalSocket *m_listener;

  /**
   * @brief Lese eingehende Nachrichten.
   * @example Das Nachrichtenformat muss in Jsonformat vorliegen!
   * @code
   *  QJsonObject {
   *    "receiver":"String",
   *    "type":"String",
   *    "value":"String"
   *  }
   * @endcode
   */
  void readClientConnection();

protected:
  void incomingConnection(quintptr descriptor) override;

Q_SIGNALS:
  /**
   * @brief Wird aufgerufen wenn das Programm bereits gestartet ist.
   * Soll das Fenster wieder in die Programmleiste setzen wenn es im
   * Benachrichtigungsfeld versteckt ist.
   */
  void showWindow();

  /**
   * @brief Status Nachrichten an Fenster oder Benachrichtigungsfeld senden.
   */
  void sendStatusMessage(const QString &);

  /**
   * @brief System Benachrichtigungsfeld Nachrichten
   */
  void sendSystemMessage(Antiqua::ErrorStatus, const QString &);

public:
  explicit SocketServer(QObject *parent = nullptr);

  /**
   * @brief Name für Klientverbindungen
   */
  static const QString name();
};

#endif // SOCKETSERVER_H
