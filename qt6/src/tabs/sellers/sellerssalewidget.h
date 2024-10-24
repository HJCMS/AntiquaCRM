// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_SALESWIDGET_H
#define ANTIQUACRM_PLUGIN_SELLERS_SALESWIDGET_H

#include <AntiquaWidgets>
#include <QTabWidget>
#include <QToolBar>

class PurchaseHeader;
class PurchaseTable;
class PurchaseActionBar;
class SellersTabWidget;
class BuyerInfo;
class Creditworthiness;
class SalesInfo;

class SellersSalesWidget final : public QWidget {
  Q_OBJECT

private:
  Q_DISABLE_COPY(SellersSalesWidget);
  const QJsonObject p_order;
  QTabWidget* m_tab;
  PurchaseHeader* m_header;
  BuyerInfo* m_buyerInfo;
  SalesInfo* m_salesInfo;
  Creditworthiness* m_worthiness;
  PurchaseTable* m_table;
  PurchaseActionBar* m_actionBar;
  const QString mediaType(const QJsonValue&);
  AntiquaCRM::ArticleType getArticleType(qint64 aid);
  const QString getPrice(const QJsonValue&) const;
  const QString getTitle(const QJsonValue&) const;
  bool findCustomer(const QJsonObject& object);
  void pushCmd(const QJsonObject& action);

private Q_SLOTS:
  void openOrder(qint64 oid);
  void createOrder(const QString& prid);
  void openCustomer();
  void openArticle(qint64 aid);
  void createProviderActions();
  void findArticleIds();
  void prepareCreateOrder();
  void setTrustStatus(AntiquaCRM::CustomerTrustLevel tl);

Q_SIGNALS:
  void sendOpenRemoteAction(const QJsonObject&);

public:
  explicit SellersSalesWidget(const QJsonObject& config, QWidget* parent = nullptr);
  bool init();
};

#endif
