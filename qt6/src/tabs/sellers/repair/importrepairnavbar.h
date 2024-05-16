// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_SELLERS_REPAIR_NAVBAR_H
#define ANTIQUACRM_SELLERS_REPAIR_NAVBAR_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

class ImportRepairNavBar : public QWidget {
  Q_OBJECT

private:
  QPushButton *btn_back;
  QPushButton *btn_next;

Q_SIGNALS:
  void sendPrevPage();
  void sendNextPage();

public Q_SLOTS:
  void setCurrentIndex(int index = -1);

public:
  explicit ImportRepairNavBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_SELLERS_REPAIR_NAVBAR_H
