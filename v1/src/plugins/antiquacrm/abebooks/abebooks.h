// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_LIBRARY_H
#define ABEBOOKS_LIBRARY_H

#include <AntiquaCRMPlugin>
#include <QDomElement>
#include <QObject>

class AbeBooksDocument;

class ANTIQUACRM_LIBRARY Abebooks final : public AntiquaCRM::APluginInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_INTERFACE FILE "abebooks.json")
  Q_INTERFACES(AntiquaCRM::APluginInterface)

private:
  bool initConfigurations() override;
  AbeBooksDocument initDocument();
  const QUrl apiQuery(const QString &section) override;
  const QString
  dateString(const QDate &date = QDate::currentDate()) const override;

private Q_SLOTS:
  void authenticate() override {/* unused */};
  void prepareResponse(const QJsonDocument &js) override;
  void prepareResponse(const QDomDocument &xml) override;
  void queryFinished(QNetworkReply *reply) override;

public Q_SLOTS:
  void orderUpdateAction(const QJsonObject &options) override;

public:
  explicit Abebooks(QObject *parent = nullptr);
  bool authenticationRequired() override { return false; };
  AntiquaCRM::UpdateDialog *updateDialog(QWidget *parent) override;
  const QString configProvider() const override;
  const QString displayName() const override;
  bool createInterface(QObject *parent) override;
};

#endif
