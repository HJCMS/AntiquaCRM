// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_HELPER_DIALOG_H
#define ANTIQUACRM_HELPER_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QObject>
#include <QLayout>
#include <QPushButton>
#include <QWidget>

/**
 * @brief The HelperDialog class
 * @section AntiquaHelper
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
