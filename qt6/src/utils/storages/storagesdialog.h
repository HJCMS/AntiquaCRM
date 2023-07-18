// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGESDIALOG_H
#define ANTIQUACRM_UTILS_STORAGESDIALOG_H

#include <AntiquaWidgets>
#include <QDialog>

class StoragesDialog final : public QDialog {
 Q_OBJECT

public:
  explicit StoragesDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif
