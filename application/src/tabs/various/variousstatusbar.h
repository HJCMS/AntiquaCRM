// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_VARIOUSSTATUSBAR_H
#define ANTIQUACRM_VARIOUSSTATUSBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Inventory Various Status bar
 * @ingroup _various
 */
class VariousStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit VariousStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_VARIOUSSTATUSBAR_H
