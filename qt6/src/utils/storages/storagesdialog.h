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
#include <QAction>
#include <QDialog>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QToolBar>

class StorageTable;
class StorageEditorWidget;

class StoragesDialog final : public QDialog {
  Q_OBJECT

private:
  QToolBar *m_toolBar;
  QAction *ac_add;
  QLineEdit *m_search;
  QAction *ac_search;
  StorageTable *m_table;
  StorageEditorWidget *m_editorWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void searchClicked();
  void saveClicked();

public:
  explicit StoragesDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif
