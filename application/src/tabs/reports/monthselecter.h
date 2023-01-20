// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MONTH_SELECTER_H
#define ANTIQUACRM_MONTH_SELECTER_H

#include <AntiquaInputEdit>
#include <QCalendar>
#include <QDate>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>
#include <QWidget>

/**
 * @brief Auswahlleiste für den Monatsbericht
 * @class MonthSelecter
 * @section widgets
 * @code
 *  {Messanger | SelectBox | Submit Button | Print Button | Export Button}
 * @endcode
 */
class MonthSelecter final : public QStatusBar {
  Q_OBJECT

private:
  /**
   * @brief Kalender für die Berechnung
   */
  QCalendar p_cal;

  /**
   * @brief Anzahl der Monate in die Vergangenheit (months to the past)
   */
  qint8 mttp = 6;

  /**
   * @brief Monatsauswahl
   */
  AntiquaComboBox *m_comboBox;

  /**
   * @brief Erstellen Knopf
   */
  QPushButton *btn_startReport;

  /**
   * @brief Drucker Dialog öffnen
   */
  QPushButton *btn_printReport;

  /**
   * @brief CSV Erstellen
   */
  QPushButton *btn_saveReport;

  /**
   * @brief Erstellt die das Auswahlmenü
   */
  void createSelectionRange();

private Q_SLOTS:
  /**
   * @brief Datum ermitteln wenn erstellen gedrückt wurde!
   */
  void createPressed();

Q_SIGNALS:
  /**
   * @brief Sendet das Monatsdatum mit dem letzen Tag im Monat!
   */
  void sendMonthDate(const QDate &);

  /**
   * @brief Weiterleitung - Drucker Dialog öffnen
   */
  void sendPrintReport();

  /**
   * @brief Weiterleitung - CSV Export Dialog öffnen
   */
  void sendSaveReport();

public:
  explicit MonthSelecter(QWidget *parent = nullptr);

  /**
   * @brief Aktueller Auswahltext der ComboBox
   */
  const QString selectedText();

  /**
   * @brief Aktuelles Datum der Auswahlbox
   * @note Eine Datumsprüfung muss selbst ausgeführt werden!
   */
  const QDate selectedMonth();
};

#endif // ANTIQUACRM_MONTH_SELECTER_H
