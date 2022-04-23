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
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;
  bool modified;
  QDoubleSpinBox *m_box;

private Q_SLOTS:
  void itemChanged(double);

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  Q_INVOKABLE void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit PriceEdit(QWidget *parent = nullptr);
  void setMinimum(double);
  void setRequired(bool b);
  bool isRequired();
  void setValue(const QVariant &);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // PRICEEDIT_H
