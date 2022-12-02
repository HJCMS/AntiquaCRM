// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ANTIQUACMD_ADAPTOR_H
#define ANTIQUACRM_ANTIQUACMD_ADAPTOR_H

#include <AGlobal>
#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QString>
#include <QStringList>

class AntiquaCMDAdaptor final : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", ANTIQUACRM_CONNECTION_DOMAIN)
  Q_PROPERTY(QString help READ help CONSTANT)
  Q_PROPERTY(QString status READ status CONSTANT)
  Q_PROPERTY(QStringList providers READ providers CONSTANT)
  Q_PROPERTY(QStringList actions READ actions CONSTANT)

public:
  explicit AntiquaCMDAdaptor(QObject *parent = nullptr);
  const QString help();
  const QString status();
  const QStringList providers();
  const QStringList actions();
  virtual ~AntiquaCMDAdaptor();

Q_SIGNALS:
  /**
   * @brief Send Command Execution
   */
  void sendExecution(const QString &provider, const QString &action);
  void sendQueryUpdate();

public Q_SLOTS:
  Q_NOREPLY void quit();

  /**
   * @brief Provider operation call
   * @note Call: "qdbus de.hjcms.antiquacrm / providers" or "qdbus
   * de.hjcms.antiquacrm / actions" to get the right parameters
   */
  Q_NOREPLY void command(const QString &provider, const QString &action);

  /**
   * @brief Query all Orders from registered Providers
   */
  Q_NOREPLY void queryOrders();
};

#endif // ANTIQUACRM_ANTIQUACMD_ADAPTOR_H
