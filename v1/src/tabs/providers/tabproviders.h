// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABPROVIDERS_H
#define ANTIQUACRM_TABPROVIDERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QSplitter>
#include <QScrollArea>
#include <QStatusBar>

class ProvidersPageView;
class ProvidersOrderPage;
class ProvidersTreeView;

class TabProviders final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  QSplitter *m_splitter;
  ProvidersPageView *m_pages;
  ProvidersTreeView *m_treeWidget;
  // QStatusBar *m_statusBar;
  QScrollArea *m_editorPage;

  bool findPage(const QString &provider, const QString &orderId);

private Q_SLOTS:
  void openOrderPage(const QString &provider, const QString &orderId);

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 customerId) override;
  void onEnterChanged() override;

public:
  explicit TabProviders(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABPROVIDERS_H
