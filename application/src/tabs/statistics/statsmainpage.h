// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATSMAINPAGE_H
#define ANTIQUACRM_STATSMAINPAGE_H

#include <AntiquaWidgets>
#include <QComboBox>
#include <QDate>
#include <QObject>
#include <QStatusBar>
#include <QTextEdit>
#include <QWidget>

class StatsMainPage : public QWidget {
  Q_OBJECT

private:
  QDate p_startDate;
  QComboBox *m_chartSelect;
  QStatusBar *m_statusBar;
  YearSelect *m_yearSelect;
#ifdef ANTIQUA_DEVELOPEMENT
  QTextEdit *m_textInfo;
#endif

private Q_SLOTS:
  void createSelectionInfo(int);
  void createChartAction();

Q_SIGNALS:
  void sendOpenChart(const QString &name, const QDate &date);

public:
  explicit StatsMainPage(QWidget *parent = nullptr);
  const QDate selectedDate();
};

#endif // ANTIQUACRM_STATSMAINPAGE_H
