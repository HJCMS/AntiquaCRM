// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_EDIT_H
#define DESIGNATIONS_EDIT_H

#include <QLabel>
#include <QRegExp>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

#include "designationdata.h"

/**
 * @brief The DesignationEdit class
 * @ingroup utils
 */
class DesignationEdit : public QWidget {
  Q_OBJECT

private:
  QSpinBox *m_id;
  QLineEdit *m_keyword;
  QTextEdit *m_description;
  DesignationData origin_t;
  inline QLabel *label(const QString &);
  const QRegExp spaces() const;
  const QString getKeyword();
  const QString getDesciption();
  void createSqlInsert();
  void createSqlUpdate();

public Q_SLOTS:
  void setClear();
  void setCreateNew();
  void setDelete();
  void setSave();
  void setEditItem(const DesignationData &);

public:
  explicit DesignationEdit(QWidget *parent = nullptr);

Q_SIGNALS:
  void sendStatusMessage(const QString &);
  void sendSaveQuery(const QString &);
};

#endif // DESIGNATIONS_EDIT_H
