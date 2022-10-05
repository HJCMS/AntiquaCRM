// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKBINDING_H
#define ANTIQUACRM_BOOKBINDING_H

#include <QWidget>

#include "inputedit.h"

class @CLASSNAME@ : public InputEdit {
  Q_OBJECT

private:
  // Qt classes

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit @CLASSNAME@(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_BOOKBINDING_H
