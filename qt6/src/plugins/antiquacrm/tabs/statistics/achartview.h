// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_ACHARTVIEW_H
#define ANTIQUACRM_STATISTICS_ACHARTVIEW_H

#include <AntiquaCRM>
#include <QChartView>
#include <QPieSeries>
#include <QDateTime>
#include <QFont>
#include <QObject>
#include <QSqlQuery>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY AChartView : public QChartView {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;

protected:
  /**
   * @brief Saved currency settings
   */
  QString currency;

  /**
   * @brief Label and Header fonts
   */
  QFont headersFont, labelsFont;

  /**
   * @brief Initial PieSeries
   * @param parent QChart
   */
  QPieSeries *initSeries(QChart *parent) const;

  /**
   * @brief Overload function from QDateTime::fromSecsSinceEpoch
   * @param ct = Current Timestamp
   */
  const QDateTime getEpoch(qint64 ct) const;

  /**
   * @brief Get Year from DateTime
   * @param dt - Date time
   */
  int getYear(const QDateTime &dt) const;

  /**
   * @brief Get Month from DateTime
   * @param dt - Date time
   */
  int getMonth(const QDateTime &dt) const;

  /**
   * @brief Create PgSQL with statement
   * @param query - SQL Query statement
   */
  const QSqlQuery getSqlQuery(const QString &query);

  /**
   * @brief Create PgSQL query from Template
   * @param tpl - Templatefile basename
   */
  const QSqlQuery getTplSqlQuery(const QString &tpl);

  /**
   * @brief Round prices
   * @param value - Price
   */
  double roundPrice(double value) const;

  virtual bool initialChartView(int year = -1) = 0;

public:
  explicit AChartView(QWidget *parent = nullptr);
  virtual ~AChartView();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_STATISTICS_ACHARTVIEW_H
