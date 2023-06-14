// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ATRANSMITTER_H
#define ANTIQUACRM_ATRANSMITTER_H

#include <AGlobal>
#include <QJsonObject>
#include <QLocalSocket>
#include <QObject>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief Application Socket Transmitter
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ATransmitter final : public QLocalSocket {
  Q_OBJECT

private:
  bool connected;

private Q_SLOTS:
  /**
   * @brief Debug LocalServerError
   * @param error - LocalServerError enumeration
   */
  void getErrors(QLocalSocket::LocalSocketError error);

  /**
   * @brief Debug LocalSocketState
   * @param state - LocalSocketState enumeration
   */
  void getState(QLocalSocket::LocalSocketState state);

public:
  /**
   * @param parent - Parent Windows Object
   */
  explicit ATransmitter(QObject *parent = nullptr);

  /**
   * @brief Send Socket operation to defined target.
   * @param json - defined Json Object operation
   *
   * This Slot create a QJsonDocument from given QJsonObject and send it to
   * @ref AntiquaCRM::AReceiver Socket server.
   *
   * Allowed Json structure and rules, are defined in constructor from @ref
   * AntiquaCRM::AReceiver::AReceiver
   *
   * On Windows this is a named pipe and on Unix this is a local domain socket
   *
   * @note
   * @li A list with valid operations can read from @ref
   * AntiquaCRM::AReceiver::operations
   * @li All Unknown operations are ignored.
   */
  bool pushOperation(const QJsonObject &json);

  /**
   * @brief Send a Window StatusBar Message
   * @param message - Message body
   *
   * Create a new @ref pushOperation for parent.
   *
   * @code
   *  QJsonObject obj;
   *  obj.insert("ACTION", QJsonValue("status_message"));
   *  obj.insert("TARGET", QJsonValue("window_tab"));
   *  obj.insert("VALUE", QJsonValue("Hello world!"));
   *  pushOperation(obj);
   * @endcode
   */
  bool pushStatusBarMessage(const QString &message);

  /**
   * @brief close socket connection
   *
   * Check socket state and disconnectFromServer
   */
  void close() override;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ATXSOCKET_H
