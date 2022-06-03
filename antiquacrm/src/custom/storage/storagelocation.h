// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGE_LOCATION_H
#define STORAGE_LOCATION_H

#include <QAction>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QHash>
#include <QKeyEvent>
#include <QLineEdit>
#include <QRegularExpression>
#include <QSpinBox>
#include <QStatusBar>
#include <QToolBar>

#include "storagetable.h"

class EditorWidget final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QRegularExpression pattern;
  QSpinBox *sl_id;
  QLineEdit *sl_storage;
  QLineEdit *sl_identifier;
  QLineEdit *sl_location;

  bool check(QLineEdit *);

public Q_SLOTS:
  void setValue(const StorageTable::RowValues &items);
  void clear();

public:
  EditorWidget(QWidget *parent = nullptr);
  const QString sqlQuery();
};

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
  EditorWidget *m_editorWidget;
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
