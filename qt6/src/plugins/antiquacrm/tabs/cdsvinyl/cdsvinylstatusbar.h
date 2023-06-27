// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLSTATUSBAR_H
#define ANTIQUACRM_PLUGIN_CDSVINYLSTATUSBAR_H

#include <AntiquaTabs>

class ANTIQUACRM_LIBRARY CDsVinylStatusBar final
    : public AntiquaCRM::TabsStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit CDsVinylStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLSTATUSBAR_H
