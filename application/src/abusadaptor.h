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

class ABusAdaptor final : public QDBusAbstractAdaptor {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", ANTIQUACRM_CONNECTION_DOMAIN)
  Q_PROPERTY(QString message READ message WRITE pushMessage NOTIFY received)

private:
  QString p_message;

Q_SIGNALS:
  void sendAboutQuit();
  void sendToggleView();
  void sendMessage(const QString &);
  void received();

public Q_SLOTS:
  Q_NOREPLY void toggle();
  Q_NOREPLY void quit();
  Q_NOREPLY void pushMessage(const QString &);

public:
  explicit ABusAdaptor(QObject *parent = nullptr);
  const QString message() const;
};

#endif // ANTIQUACRM_BUSADAPTOR_H
