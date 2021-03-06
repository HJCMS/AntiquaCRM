// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRICEEDIT_UTILS_H
#define PRICEEDIT_UTILS_H

#include <QObject>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QWidget>
// Personal
#include <UtilsMain>

class PriceEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;
  QDoubleSpinBox *m_box;

private Q_SLOTS:
  void itemChanged(double);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit PriceEdit(QWidget *parent = nullptr);
  void setMinimum(double);
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // PRICEEDIT_UTILS_H
