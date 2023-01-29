// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_DIALOG_H
#define DESIGNATIONS_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QStatusBar>
#include <QWidget>

class DesignationsList;
class DesignationEdit;

class DesignationsDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  DesignationsList *m_listView;
  DesignationEdit *m_edit;
  QStatusBar *m_statusBar;
  QPushButton *btn_clear;
  QPushButton *btn_delete;
  QPushButton *btn_save;
  QPushButton *btn_close;

  void keyPressEvent(QKeyEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  bool event(QEvent *) override;

  bool loadBookDesignations();

private Q_SLOTS:
  void setSaveQuery(const QString &);

public:
  explicit DesignationsDialog(QWidget *parent = nullptr);
  int exec() override;
  virtual ~DesignationsDialog();
};

#endif // DESIGNATIONS_DIALOG_H
