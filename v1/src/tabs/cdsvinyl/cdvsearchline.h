// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVSEARCHLINE_H
#define ANTIQUACRM_CDVSEARCHLINE_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CDVSearchLine final : public SearchLineEdit {
  Q_OBJECT

public:
  explicit CDVSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_CDVSEARCHLINE_H
