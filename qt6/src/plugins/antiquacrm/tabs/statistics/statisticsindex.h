// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_INDEX_H
#define ANTIQUACRM_STATISTICS_INDEX_H

#include "statisticsconfig.h"
#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class StatisticsViewArea;
class StatisticsSelecter;

class ANTIQUACRM_STATISTICS_PLUGIN StatisticsIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  const QDate p_date;
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  StatisticsViewArea* m_view;
  StatisticsSelecter* m_selecter;

  /**
   * @brief Used for clear ChartView
   */
  void setDefaultTableView() override;

public Q_SLOTS:
  void clearArea();
  void setChart(qint64 year, const QString& chart);

public:
  explicit StatisticsIndex(QWidget* parent = nullptr);
  void openStartPage() override {};
  void createSearchQuery(const QString&) override {};
  void createNewEntry() override {};
  void openEntry(qint64) override{};
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject& obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_STATISTICS_INDEX_H
