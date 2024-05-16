// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DESIGNATIONEDIT_H
#define ANTIQUACRM_UTILS_DESIGNATIONEDIT_H

#include <QLabel>
#include <QRegularExpression>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

#include "designationdata.h"

/**
 * @brief The DesignationEdit class
 * @ingroup _designations
 */
class DesignationEdit final : public QWidget {
  Q_OBJECT

private:
  QSpinBox *m_id;
  QLineEdit *m_keyword;
  QTextEdit *m_description;
  DesignationData origin_t;
  inline QLabel *label(const QString &);
  const QRegularExpression spaces() const;
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

#endif // ANTIQUACRM_UTILS_DESIGNATIONEDIT_H
