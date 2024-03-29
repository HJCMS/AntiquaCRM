// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef STORAGE_EDITDIALOG_H
#define STORAGE_EDITDIALOG_H

#include <QAction>
#include <QDialog>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QToolBar>

class StorageTable;
class StorageEditor;

/**
 * @ingroup _storage
 * @brief Storage Section
 *
 * @class StorageEditDialog
 * @brief Storage Edit Dialog
 */
class StorageEditDialog final : public QDialog {
  Q_OBJECT

private:
  QToolBar *m_toolBar;
  QAction *ac_add;
  QLineEdit *m_search;
  QAction *ac_search;
  StorageTable *m_table;
  StorageEditor *m_editorWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void searchClicked();
  void saveClicked();

public:
  explicit StorageEditDialog(QWidget *parent = nullptr);
};

#endif // STORAGE_EDITDIALOG_H
