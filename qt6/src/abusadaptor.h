// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_ABUSADAPTOR_H
#define ANTIQUA_UI_ABUSADAPTOR_H

#include <AGlobal>
#include <QDBusAbstractAdaptor>
#include <QDBusError>
#include <QDBusVariant>
#include <QObject>

/**
 * @class ABusAdaptor
 * @brief AntiquaCRM D-Bus Adaptor
 */
class ABusAdaptor final : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", ANTIQUACRM_CONNECTION_DOMAIN)
  Q_PROPERTY(QString message READ message WRITE pushMessage NOTIFY received)

private:
  QString p_message;

Q_SIGNALS:
  /**
   * @brief send signal, about to application quit
   */
  void sendAboutQuit();

  /**
   * @brief send signal, toggle main window view
   */
  void sendToggleView();

  /**
   * @brief send signal, message to window
   */
  void sendMessage(const QString &);

  /**
   * @brief send signal, operation received
   */
  void received();

public Q_SLOTS:
  /**
   * @brief toggle window view request
   */
  Q_NOREPLY void toggle();

  /**
   * @brief application quit request
   */
  Q_NOREPLY void quit();

  /**
   * @brief push message to system tray icon class
   */
  Q_NOREPLY void pushMessage(const QString &msg);

public:
  /**
   * @param parent - object handle
   */
  explicit ABusAdaptor(QObject *parent = nullptr);

  /**
   * @brief receive push message
   */
  const QString message() const;
};

#endif // ANTIQUA_UI_ABUSADAPTOR_H
