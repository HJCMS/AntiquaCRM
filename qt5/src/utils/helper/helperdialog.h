// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_HELPER_DIALOG_H
#define ANTIQUACRM_HELPER_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QObject>
#include <QLayout>
#include <QPushButton>
#include <QWidget>

/**
 * @ingroup _helper
 * @brief Helper ToolBox Dialog
 */
class HelperDialog final : public QDialog {
  Q_OBJECT

private:
  QVBoxLayout *layout;
  QWidget *m_dummy;
  QPushButton *btn_ok;

private Q_SLOTS:
  void setHelp(QWidget *);

public:
  explicit HelperDialog(QWidget *parent = nullptr);
  int exec() override;
  int exec(const QString &section);
  virtual ~HelperDialog();
}; // ANTIQUACRM_HELPER_DIALOG_H

#endif
