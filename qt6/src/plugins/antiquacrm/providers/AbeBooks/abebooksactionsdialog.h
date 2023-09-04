// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ACTIONSDIALOG_H
#define ABEBOOKS_PLUGIN_ACTIONSDIALOG_H

#include <AntiquaProviders>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>
#include <QtXml>

class AbeBooksOrderMainInfo;
class AbeBooksSellerStatus;
class AbeBooksActionsBar;

/**
 * @class AbeBooksActionsDialog
 * @brief AbeBooks Provider Actions Dialog
 * @warning AbeBooks only supports ISO-8859-1 encoding!
 *
 * @ingroup AbeBooks
 */
class ANTIQUACRM_LIBRARY AbeBooksActionsDialog final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  /**
   * @brief AbeBooks Seller Client Id
   *
   * Required for creating Remote Urls.
   */
  QString seller_id;

  /**
   * @brief This ID is the AbeBooks purchase order ID, or ABEPOID.
   */
  QString order_id;

  /**
   * @brief configuration settings
   */
  QMap<QString, QVariant> p_config;

  QStackedWidget *m_pages;
  AbeBooksOrderMainInfo *m_orderInfo;
  AbeBooksSellerStatus *m_sellerStatus;
  AbeBooksActionsBar *m_actionsBar;

  AntiquaCRM::ANetworker *m_network;

  bool initConfiguration() override;

  const QUrl apiQuery(const QString &target = QString()) override;

  QDomDocument orderUpdateRequest(const QString &attr = QString("getOrder"));

private Q_SLOTS:
  void previousPage();
  void nextPage();

  void queryFinished(QNetworkReply *reply);

  void prepareResponse(const QDomDocument &xml);

  void prepareOperation() override;

public:
  explicit AbeBooksActionsDialog(QWidget *parent = nullptr);

  int exec(const QJsonObject &data) override;
};

#endif // ABEBOOKS_PLUGIN_ACTIONSDIALOG_H
