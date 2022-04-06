// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRICEEDIT_H
#define PRICEEDIT_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QWidget>

class PriceEdit : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QDoubleSpinBox *m_box;

public:
  explicit PriceEdit(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  const QVariant value();
};

#endif // PRICEEDIT_H
