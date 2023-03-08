// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BUCHFREUND_LIBRARY_H
#define BUCHFREUND_LIBRARY_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QJsonValue>
#include <QMetaType>
#include <QObject>

/**
 * @brief The Buchfreund class
 * @ingroup AntiquaCRMPlugin
 */
class ANTIQUACRM_LIBRARY Buchfreund final
    : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "buchfreund.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  bool initConfigurations() override;
  const QUrl apiQuery(const QString &section) override;
  const QString
  dateString(const QDate &date = QDate::currentDate()) const override;

private Q_SLOTS:
  void authenticate() override{/* unused */};
  void prepareResponse(const QJsonDocument &js) override;
  void prepareResponse(const QDomDocument &) override{/* unused */};
  void queryFinished(QNetworkReply *reply) override;

public Q_SLOTS:
  void orderUpdateAction(const QJsonObject &options) override;

public:
  explicit Buchfreund(QObject *parent = nullptr);
  bool authenticationRequired() override;
  AntiquaCRM::UpdateDialog *updateDialog(QWidget *parent) override;
  const QString configProvider() const override;
  const QString displayName() const override;
  bool createInterface(QObject *parent) override;
};

#endif
