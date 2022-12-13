// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_COUPONS_DIALOG_H
#define ANTIQUACRM_COUPONS_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QObject>
#include <QWidget>

class CouponsDialog final : public QDialog {
  Q_OBJECT

public:
  explicit CouponsDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif
