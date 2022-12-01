// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABREPORTS_H
#define ANTIQUACRM_TABREPORTS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class MonthlyReports;

class TabReports final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_pageStart;
  MonthlyReports *m_monthlyReports;

  void popupWarningTabInEditMode() override {};

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override {/* disabled */};
  void openEntry(qint64) override {/* disabled */};
  void onEnterChanged() override;

public:
  explicit TabReports(QWidget *parent = nullptr);
  bool customAction(const QJsonObject &) override { return false; }
};

#endif // ANTIQUACRM_TABREPORTS_H
