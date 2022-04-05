// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRICEEDIT_H
#define PRICEEDIT_H

#include <QtCore/QObject>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QWidget>

class PriceEdit : public QDoubleSpinBox
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit PriceEdit(QWidget *parent = nullptr);

};

#endif // PRICEEDIT_H

