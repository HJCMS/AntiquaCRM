// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_KEYWORDS_DIALOG_H
#define ANTIQUACRM_UTILS_KEYWORDS_DIALOG_H

#include <AntiquaWidgets>
#include <QDialog>
#include <QPushButton>

class KeywordsTable;
class KeywordsEdit;

class KeywordsDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  QString p_queryTable;
  KeywordsTable *m_table;
  KeywordsEdit *m_edit;
  QPushButton *btn_create;
  QPushButton *btn_commit;
  const QString tableName() const;

private Q_SLOTS:
  void commit();

public:
  explicit KeywordsDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_UTILS_KEYWORDS_DIALOG_H
