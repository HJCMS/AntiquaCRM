// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DESIGNATIONDIALOG_H
#define ANTIQUACRM_UTILS_DESIGNATIONDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QKeyEvent>
#include <QStatusBar>
#include <QWidget>

class DesignationsList;
class DesignationEdit;

class DesignationsDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  DesignationsList *m_listView;
  DesignationEdit *m_edit;
  QStatusBar *m_statusBar;
  QPushButton *btn_clear;
  QPushButton *btn_delete;
  QPushButton *btn_save;
  QPushButton *btn_close;

  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

  bool loadBookDesignations();

private Q_SLOTS:
  void setSaveQuery(const QString &);

public:
  explicit DesignationsDialog(QWidget *parent = nullptr);
  int exec() override;
  virtual ~DesignationsDialog();
};

#endif // ANTIQUACRM_UTILS_DESIGNATIONDIALOG_H
