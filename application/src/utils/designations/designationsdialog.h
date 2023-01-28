// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_DIALOG_H
#define DESIGNATIONS_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QResizeEvent>
#include <QSplitter>
#include <QStatusBar>
#include <QLabel>
#include <QWidget>

class DesignationsList;
class DesignationEdit;

class DesignationsDialog final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  QLabel *m_label;
  QSplitter *m_splitter;
  DesignationsList *m_listView;
  DesignationEdit *m_edit;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;
  bool loadBookDesignations();

private Q_SLOTS:
  void setSaveQuery(const QString &);
  void resizeEvent(QResizeEvent *) override;

public:
  explicit DesignationsDialog(QWidget *parent = nullptr);
  int exec() override;
  virtual ~DesignationsDialog();
};

#endif // DESIGNATIONS_DIALOG_H
