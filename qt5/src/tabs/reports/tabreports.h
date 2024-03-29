// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABREPORTS_H
#define ANTIQUACRM_TABREPORTS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class MonthlyReports;

/**
 * @brief Inventory Reports
 * @ingroup _reports
 */
class TabReports final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_pageStart;
  MonthlyReports *m_monthlyReports;

public Q_SLOTS:
  void popupWarningTabInEditMode() override {};
  void setDefaultTableView() override {};
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override {/* disabled */};
  void openEntry(qint64) override {/* disabled */};
  void onEnterChanged() override;

public:
  explicit TabReports(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override { return MainView; };
  bool customAction(const QJsonObject &) override { return false; }
};

#endif // ANTIQUACRM_TABREPORTS_H
