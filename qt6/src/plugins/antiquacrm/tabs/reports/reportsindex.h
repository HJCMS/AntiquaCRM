// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_INDEX_H
#define ANTIQUACRM_REPORTS_INDEX_H

#include <AntiquaTabs>
#include <QDate>
#include <QObject>
#include <QWidget>

class ReportsTableView;
class ReportsToolBar;

class ANTIQUACRM_LIBRARY ReportsIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QDate p_date;
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  ReportsTableView *m_table;
  ReportsToolBar *m_toolBar;
  void setDefaultTableView() override;
  const QString getBasename();
  const QString printHeader();
  const QFileInfo getSaveFile();

private Q_SLOTS:
  /**
   * @brief Erstelle einen Bericht für den Monat
   * @param date - ISO-8601
   * @see ReportAction::lastDayInMonth
   *
   * Das Datum muss immer den letzten Tag des Monats enthalten!
   */
  void createReport(const QDate &date);

  /**
   * @brief Standard Drucken Dialog
   */
  void printReport();

  /**
   * @brief Standard CSV Export/Speichern
   */
  void saveReport();

public:
  explicit ReportsIndex(QWidget *parent = nullptr);
  void openStartPage() override{};
  void createSearchQuery(const QString &) override{};
  void createNewEntry() override{};
  void openEntry(qint64) override{};
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject &obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_REPORTS_INDEX_H
