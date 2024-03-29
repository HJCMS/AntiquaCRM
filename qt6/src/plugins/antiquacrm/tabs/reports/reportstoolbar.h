// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_TOOLBAR_H
#define ANTIQUACRM_REPORTS_TOOLBAR_H

#include <AntiquaCRM>
#include <QDate>
#include <QLabel>
#include <QMenu>
#include <QObject>
#include <QPushButton>
#include <QToolBar>
#include <QWidget>

class ANTIQUACRM_LIBRARY ReportAction final : public QAction {
  Q_OBJECT

private:
  const QDate p_date;

private Q_SLOTS:
  void clicked();

Q_SIGNALS:
  void signalClicked(const QDate &);

public:
  explicit ReportAction(const QIcon &icon, const QDate &date,
                        QObject *parent = nullptr);
};

class ANTIQUACRM_LIBRARY ReportsToolBar final : public QToolBar {
  Q_OBJECT

private:
  const QIcon p_icon;
  QLabel *m_info;
  QPushButton *m_save;
  QPushButton *m_print;
  QPushButton *m_months;
  QMenu *m_menu;

private Q_SLOTS:
  void selectionChanged(const QDate &);

Q_SIGNALS:
  void signalSelected(const QDate &);
  void sendPrintReport();
  void sendSaveReport();

public Q_SLOTS:
  void updateInfoLabel(const QDate &date, double volume = 0.00);

public:
  explicit ReportsToolBar(QWidget *parent = nullptr);
  const QDate firstDayInMonth(const QDate &date) const;
  const QDate lastDayInMonth(const QDate &date) const;
  bool initSelection(AntiquaCRM::ASqlCore *);
};

#endif // ANTIQUACRM_REPORTS_TOOLBAR_H
