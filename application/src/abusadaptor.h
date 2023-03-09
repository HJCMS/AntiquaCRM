// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BUSADAPTOR_H
#define ANTIQUACRM_BUSADAPTOR_H

#include <AGlobal>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QDBusError>

/**
 * @ingroup ui
 * @brief AntiquaCRM D-Bus Adaptor
 * @note GNU/LINUX only
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
  Q_NOREPLY void pushMessage(const QString &);

public:
  explicit ABusAdaptor(QObject *parent = nullptr);

  /**
   * @brief receive push message
   */
  const QString message() const;
};

#endif // ANTIQUACRM_BUSADAPTOR_H
