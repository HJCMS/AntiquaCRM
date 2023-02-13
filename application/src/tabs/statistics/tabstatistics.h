// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSTATISTICS_H
#define ANTIQUACRM_TABSTATISTICS_H

#include <AntiquaWidgets>
#include <QDate>
#include <QObject>
#include <QWidget>

class StatsMainPage;

class TabStatistics final : public Inventory {
  Q_OBJECT

private:
  StatsMainPage *m_mainPage;
  bool initialed = false;

private Q_SLOTS:
  void openChartView(const QString &name, const QDate &date);
  void popupWarningTabInEditMode() override{/* unused */};
  void setDefaultTableView() override{/* unused */};

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override{/* unused */};
  void openEntry(qint64) override{/* unused */};
  void onEnterChanged() override;

public:
  explicit TabStatistics(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override { return MainView; };
  bool customAction(const QJsonObject &obj) override;
  ~TabStatistics();
};

#endif // ANTIQUACRM_TABSTATISTICS_H
