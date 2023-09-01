// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_ACTIONS_H
#define BOOKLOOKER_PLUGIN_ACTIONS_H

#include <AntiquaCRM>
#include <AntiquaProviders>
#include <QLabel>
#include <QMap>
#include <QNetworkCookie>
#include <QObject>
#include <QWidget>

class BookLookerOperations;

class ANTIQUACRM_LIBRARY BookLookerActions final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  QString order_id;
  QMap<QString, QVariant> p_config;
  QLabel *m_buyerInfo;
  BookLookerOperations *m_selecter;
  AntiquaCRM::ANetworker *m_network;

  /**
   * @brief load configuration
   */
  bool initConfiguration() override;

  /**
   * @brief Session cookie
   */
  QNetworkCookie actionsCookie;

  /**
   * @brief Authentic cookie
   */
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief Operations cache cookie
   */
  QNetworkCookie cacheCookie;

  const QUrl apiQuery(const QString &target) override;

  void setTokenCookie(const QString &token);
  bool isCookieExpired();

private Q_SLOTS:
  void authenticate();
  void prepareResponse(const QJsonDocument &document);
  void orderUpdateAction(const QJsonObject &data);
  void prepareOperation() override;
  void queryFinished(QNetworkReply *reply);

Q_SIGNALS:
  void sendPostOperation(const QJsonObject &);

public:
  explicit BookLookerActions(QWidget *parent);
  int exec(const QJsonObject &data) override;
};

#endif // BOOKLOOKER_PLUGIN_ACTIONS_H
