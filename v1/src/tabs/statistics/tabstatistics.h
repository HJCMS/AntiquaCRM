// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSTATISTICS_H
#define ANTIQUACRM_TABSTATISTICS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QDate>
#include <QWidget>

class TabStatistics final : public Inventory {
  Q_OBJECT

private:
  bool firstview = false;

private Q_SLOTS:
  void openPaymentsInYear(const QDate &d);
  void popupWarningTabInEditMode() override;
  void setDefaultTableView() override;

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override{/* unused */};
  void openEntry(qint64 statsId) override;
  void onEnterChanged() override;

public:
  explicit TabStatistics(QWidget *parent = nullptr);
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABSTATISTICS_H
