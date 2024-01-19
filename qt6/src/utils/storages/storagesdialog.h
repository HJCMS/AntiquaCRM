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
#include <QPushButton>

class StorageTable;
class StorageEditorWidget;
class StorageSearchBar;

class StoragesDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  StorageSearchBar *m_searchBar;
  StorageTable *m_table;
  StorageEditorWidget *m_editorWidget;
  QPushButton *btn_create;

private Q_SLOTS:
  void save();

public:
  explicit StoragesDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif
