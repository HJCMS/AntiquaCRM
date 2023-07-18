// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_KEYWORDSDIALOG_H
#define ANTIQUACRM_UTILS_KEYWORDSDIALOG_H

#include <AntiquaWidgets>
#include <QDialog>

class KeywordsDialog final : public QDialog {
 Q_OBJECT

public:
  explicit KeywordsDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif
