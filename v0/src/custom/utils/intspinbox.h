// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INTSPINBOX_UTILS_H
#define INTSPINBOX_UTILS_H

#include <QObject>
#include <QVariant>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>
// Personal
#include <UtilsMain>

class IntSpinBox final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int min = 0; /**< Wir verwenden keine Netgativ Werte */
  int max = 9999; /**< Startwert siehe @ref setRange */
  QLabel *m_info;
  QSpinBox *m_spinBox;
  void p_construct();

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit IntSpinBox(QWidget *parent = nullptr);
  IntSpinBox(int minimum, int maximum, QWidget *parent = nullptr);
  ~IntSpinBox();

  void setRange(int minimum, int maximum);
  void setSingleStep(int);
  void setSuffix(const QString &);
  void setPrefix(const QString &);
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // INTSPINBOX_UTILS_H
