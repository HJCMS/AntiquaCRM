// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSSTATUSBAR_H
#define ANTIQUACRM_PLUGIN_BOOKSSTATUSBAR_H

#include <AntiquaTabs>
#include <QPushButton>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooksStatusBar final
    : public AntiquaCRM::TabsStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;
  void setHistoryActionMenu(QPushButton *parent) override;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit BooksStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSSTATUSBAR_H
