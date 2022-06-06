// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ECURRENCY_UTILS_H
#define ECURRENCY_UTILS_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

#include <UtilsMain>

class Ecurrency final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;
  QLineEdit *m_lineEdit;
  QComboBox *m_comboBox;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &);

public:
  explicit Ecurrency(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ECURRENCY_UTILS_H
