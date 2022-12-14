// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABOUT_QRENCODE_H
#define ANTIQUACRM_ABOUT_QRENCODE_H

#include "infowidget.h"
#include <QWidget>

class AboutQREncode final : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutQREncode(QWidget *parent = nullptr);
  void setInfoWidget() override;
};

#endif // ANTIQUACRM_ABOUT_QRENCODE_H
