// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_ACTIONS_H
#define BOOKLOOKER_PLUGIN_ACTIONS_H

#include <AGlobal>
#include <AntiquaProviders>
#include <QLabel>
#include <QMap>
#include <QNetworkCookie>
#include <QObject>
#include <QWidget>

class BookLookerOperations;

/**
 * @class BookLookerActions
 * @brief BookLooker Provider Actions Dialog
 *
 * @ingroup BookLooker
 */
class ANTIQUACRM_LIBRARY BookLookerActions final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  /**
   * @brief important order id
   */
  QString order_id;

  /**
   * @brief configuration settings
   */
  QMap<QString, QVariant> p_config;

  /**
   * @brief infos about this dialog
   */
  QLabel *m_buyerInfo;

  /**
   * @brief radio buttons list
   */
  BookLookerOperations *m_selecter;

  /**
   * @brief network queries
   */
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

  /**
   * @brief Default API Url
   * @param target - destination query
   *
   * e.g. https://api.booklooker.de/2.0/<target>
   */
  const QUrl apiQuery(const QString &target) override;

  /**
   * @brief Set new/update token
   * @param token
   */
  void setTokenCookie(const QString &token);

  /**
   * @brief Checks the cookie expiration
   */
  bool isCookieExpired();

private Q_SLOTS:
  /**
   * @brief Creates authentication request
   */
  void authenticate();

  /**
   * @brief Creates treatment for the return data.
   * @param document
   */
  void prepareResponse(const QJsonDocument &document);

  /**
   * @brief Create network query with API Url, JSON object.
   * @param data
   */
  void orderUpdateAction(const QJsonObject &data);

  /**
   * @brief Creates the selected API json action
   */
  void prepareOperation() override;

  /**
   * @brief creates status message from response
   * @param reply
   */
  void queryFinished(QNetworkReply *reply);

public:
  /**
   * @param parent - parent object
   */
  explicit BookLookerActions(QWidget *parent);

  /**
   * @brief start open dialog
   * @param data - current order data
   */
  int exec(const QJsonObject &data) override;
};

#endif // BOOKLOOKER_PLUGIN_ACTIONS_H
