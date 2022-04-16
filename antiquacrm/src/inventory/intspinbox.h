// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INTSPINBOX_H
#define INTSPINBOX_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

class IntSpinBox : public QSpinBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool modified;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit IntSpinBox(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
};

#endif // INTSPINBOX_H
