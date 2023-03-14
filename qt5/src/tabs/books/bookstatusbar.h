// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_BOOKSTATUSBAR_H
#define ANTIQUACRM_BOOKSTATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QObject>
#include <QWidget>

/**
 * @brief Books Status bar
 * @ingroup _books
 */
class BookStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit BookStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_BOOKSTATUSBAR_H
