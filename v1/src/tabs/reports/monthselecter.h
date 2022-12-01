// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTH_SELECTER_H
#define ANTIQUACRM_MONTH_SELECTER_H

#include <AntiquaInputEdit>
#include <QDate>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>
#include <QWidget>

class MonthSelecter final : public QStatusBar {
  Q_OBJECT

private:
  AntiquaComboBox *m_comboBox;
  QPushButton *btn_startReport;
  QPushButton *btn_printReport;
  QPushButton *btn_saveReport;
  void createSelectionRange();

private Q_SLOTS:
  void createPressed();

Q_SIGNALS:
  /**
   * @brief Sendet das Monatsdatum mit dem letzen Tag im Monat!
   */
  void sendMonthDate(const QDate &);
  void sendPrintReport();
  void sendSaveReport();

public:
  explicit MonthSelecter(QWidget *parent = nullptr);
  const QString selectedText();
  const QDate selectedMonth();
};

#endif // ANTIQUACRM_MONTH_SELECTER_H
