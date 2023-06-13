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
  void getErrors(QLocalSocket::LocalSocketError error);
  void getState(QLocalSocket::LocalSocketState state);

public:
  explicit ATransmitter(QObject *parent = nullptr);

  /**
   * @brief Send operations to tab
   * @param obj
   */
  bool pushOperation(const QJsonObject &obj);

  /**
   * @brief Send a Window StatusBar Message
   */
  bool pushStatusBarMessage(const QString &message);

  /**
   * @brief close socket connection
   */
  void close() override;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ATXSOCKET_H
