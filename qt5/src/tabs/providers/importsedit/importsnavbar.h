// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROVIDER_IMPORTS_NAVBAR_H
#define PROVIDER_IMPORTS_NAVBAR_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

class ImportsNavBar final : public QWidget {
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
  explicit ImportsNavBar(QWidget *parent = nullptr);
};

#endif // PROVIDER_IMPORTS_NAVBAR_H
