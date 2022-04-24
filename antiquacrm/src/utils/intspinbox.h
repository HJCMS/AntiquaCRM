// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INTSPINBOX_UTILS_H
#define INTSPINBOX_UTILS_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
// Personal
#include <UtilsMain>

class IntSpinBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;
  QSpinBox *m_spinBox;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();

public:
  explicit IntSpinBox(QWidget *parent = nullptr);
  void setRange(int min, int max);
  void setSingleStep(int);
  void setSuffix(const QString &);
  void setPrefix(const QString &);
  void setValue(const QVariant &);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // INTSPINBOX_UTILS_H
