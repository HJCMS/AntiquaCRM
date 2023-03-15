// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABPROVIDERS_H
#define ANTIQUACRM_TABPROVIDERS_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <AntiquaWidgets>
#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QTimerEvent>

class ProvidersPageView;
class ProvidersOrderPage;
class ProvidersTreeView;

/**
 * @brief Inventory Providers Index
 * @ingroup _providers
 */
class TabProviders final : public Inventory {
  Q_OBJECT

private:
  bool firstStart = false;

  AntiquaCRM::ASqlCore *m_sql;

  QWidget *m_mainPage;
  QSplitter *m_splitter;
  ProvidersPageView *m_pages;

  QFrame *m_rightFrame;
  ProvidersTreeView *m_treeWidget;
  QPushButton *btn_refreshTree;

  /**
   * @brief Alle geladenen Plugins
   */
  QList<AntiquaCRM::APluginInterface *> plugins;

  bool findPage(const QString &provider, const QString &orderId);

  /**
   * @brief Hier werden alle verfügbaren Dienstleister Plugins geladen.
   */
  bool loadPlugins();

private Q_SLOTS:
  void popupWarningTabInEditMode() override {};
  void setDefaultTableView() override {};
  void createProviderAction();
  void pluginErrorResponse(QMessageBox::Icon, const QString &);
  void pluginQueryFinished();
  void openOrderPage(const QString &provider, const QString &orderId);

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 customerId) override;
  void onEnterChanged() override;

public:
  explicit TabProviders(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override { return MainView; };
  bool customAction(const QJsonObject &) override { return false; };
  virtual ~TabProviders();
};

#endif // ANTIQUACRM_TABPROVIDERS_H
