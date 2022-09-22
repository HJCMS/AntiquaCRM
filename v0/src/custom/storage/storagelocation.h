// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGE_LOCATION_H
#define STORAGE_LOCATION_H

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

class StorageLocation final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QToolBar *m_toolBar;
  QAction *ac_add;
  QLineEdit *m_search;
  QAction *ac_search;
  StorageTable *m_table;
  StorageEditor *m_editorWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  void keyPressEvent(QKeyEvent *);
  bool event(QEvent *);

private Q_SLOTS:
  void searchClicked();
  void saveClicked();

public:
  explicit StorageLocation(QWidget *parent = nullptr);
};

#endif // STORAGE_LOCATION_H
