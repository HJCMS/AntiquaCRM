// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_EDIT_H
#define DESIGNATIONS_EDIT_H

#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QTextEdit>
#include <QStatusBar>
#include <QPushButton>
#include <QWidget>

#include "designationdata.h"

class DesignationEdit : public QWidget {
  Q_OBJECT

private:
  QComboBox *m_type;
  QLineEdit *m_keyword;
  QTextEdit *m_description;
  QStatusBar *m_statusBar;
  QPushButton *btn_save;
  DesignationData origin_t;

private Q_SLOTS:
  void setSave();

public Q_SLOTS:
  void setClear();
  void setMessage(const QString &);
  void setEditItem(const DesignationData &);

public:
  explicit DesignationEdit(QWidget *parent = nullptr);

Q_SIGNALS:
  void sendSaveQuery(const QString &);
};

#endif // DESIGNATIONS_EDIT_H
