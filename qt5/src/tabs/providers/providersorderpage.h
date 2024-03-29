// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERSORDERPAGE_H
#define ANTIQUACRM_PROVIDERSORDERPAGE_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QTextEdit>
#include <QWidget>

class ProviderPurchaseHeader;
class ProvidersPageView;
class ProviderPurchaseTable;
class ProviderPurchaseBar;
class ProviderBuyerInfo;
class ProviderOrderInfo;

/**
 * @ingroup _providers
 * @brief Provider Order/Payment Overview page.
 *
 * @class ProvidersOrderPage
 * This Widget display the complete Information about the Order. It also
 * contains some subrequest before a new order can be imported into the system.
 */
class ProvidersOrderPage final : public QWidget {
  Q_OBJECT

private:
  Q_DISABLE_COPY(ProvidersOrderPage);
  const QJsonObject p_order;
  AntiquaCRM::ASqlCore *m_sql;
  ProviderPurchaseHeader *m_header;
  ProvidersPageView *m_tab;
  ProviderPurchaseTable *m_table;
  ProviderPurchaseBar *m_actionBar;

  /**
   * @brief Detailed Order buyer infos.
   */
  ProviderBuyerInfo *m_buyerInfo;

  /**
   * @brief Detailed Provider Order infos.
   */
  ProviderOrderInfo *m_orderInfo;

  /**
   * @brief Pushing command line to the Socket server.
   */
  void pushCmd(const QJsonObject &action);

  /**
   * @brief find Customer with given arguments
   */
  bool findCustomer(const QJsonObject &customer);

  AntiquaCRM::ArticleType getArticleType(qint64);

  const QString mediaType(const QJsonValue &);

  const QString convertPrice(double) const;

private Q_SLOTS:
  /**
   * @brief Send Action Open Order with Inventory Order ID
   */
  void openOrder(qint64 oid);

  /**
   * @brief Send Action Craete Order with Provider Order ID
   */
  void createOrder(const QString &providerId);

  /**
   * @brief Create Action to open Customer.
   */
  void createOpenCustomer();

  /**
   * @brief Send Action Open Article by Article Id
   */
  void openArticle(qint64 aid);

  /**
   * @brief Send Action Open Cards Printing
   */
  void openPrinting(qint64 aid);

  /**
   * @brief SQL Query func_get_article_count()
   */
  void findArticleIds();

  /**
   * @brief Search for Existing Orders, if not exists create it ...
   */
  void prepareCreateOrder();

Q_SIGNALS:
  void sendOpenProviderDialog();

public:
  /**
   * @brief ProvidersOrderPage
   * @param order - Json Object with complete Order data.
   * @param parent
   */
  explicit ProvidersOrderPage(const QJsonObject &order,
                              QWidget *parent = nullptr);

  /**
   * @brief Load Interface data from Json Object order.
   * @note This function must called behind class initialisation!
   */
  bool loadOrderDataset();
};

#endif // ANTIQUACRM_PROVIDERSORDERPAGE_H
