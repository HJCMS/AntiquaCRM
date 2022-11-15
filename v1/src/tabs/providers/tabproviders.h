// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABPROVIDERS_H
#define ANTIQUACRM_TABPROVIDERS_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QSplitter>
#include <QTimerEvent>

class ProvidersPageView;
class ProvidersOrderPage;
class ProvidersTreeView;

class TabProviders final : public Inventory {
  Q_OBJECT

private:
  int timerId;
  int counter;
  const int resetCounter = 300; /**< steps in Seconds */

  QWidget *m_mainPage;
  QSplitter *m_splitter;
  ProvidersPageView *m_pages;

  QFrame *m_rightFrame;
  ProvidersTreeView *m_treeWidget;
  QLabel *m_timerDisplay;

  void timerEvent(QTimerEvent *event) override;

  void popupWarningTabInEditMode() override{};

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
  bool customAction(const QJsonObject &obj) override { return false; }
  ~TabProviders();
};

#endif // ANTIQUACRM_TABPROVIDERS_H
