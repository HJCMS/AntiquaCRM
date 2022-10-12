// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_INTERFACE_H
#define ANTIQUACRM_PLUGIN_INTERFACE_H

#include <AGlobal>
#include <QJsonDocument>
#include <QObject>
#include <QString>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARAY APluginInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM Plugin Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

protected Q_SLOTS:
  virtual void prepareJsonResponse(const QJsonDocument &) = 0;

Q_SIGNALS:
  void sendJsonResponse(const QJsonDocument &);
  void sendErrorResponse(AntiquaCRM::Message, const QString &);
  void sendQueryFinished(bool successfull = false);

public Q_SLOTS:
  virtual void queryOrders() = 0;

public:
  explicit APluginInterface(QObject *parent = nullptr);

  static const QString configPrefix();

  virtual const QString configProvider() const = 0;

  virtual const QString displayName() const = 0;

  virtual bool createInterface(QObject *parent) = 0;
};

}; // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::APluginInterface, ANTIQUACRM_INTERFACE)

#endif // ANTIQUACRM_PLUGIN_INTERFACE_H
