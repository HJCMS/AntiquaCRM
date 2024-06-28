// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H
#define ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H

#include <AntiquaWidgets>
#include <QStackedWidget>
#include <QLabel>

class ChangeOrderCustomerDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  QStackedWidget* m_mainWidget;
  QLabel* warnPage() const;

private Q_SLOTS:
  void confirmd();

public:
  explicit ChangeOrderCustomerDialog(QWidget* parent = nullptr);
  QDialog::DialogCode start(qint64 id);
};

#endif // ANTIQUACRM_PLUGIN_CHANGEORDERCUSTOMERDIALOG_H
