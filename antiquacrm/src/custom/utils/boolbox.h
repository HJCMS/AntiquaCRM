// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOLBOX_UTILS_H
#define BOOLBOX_UTILS_H

#include <QObject>
#include <QCheckBox>
#include <QWidget>

#include <UtilsMain>

class BoolBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QCheckBox *m_checkBox;

private Q_SLOTS:
  void itemChanged(int);

Q_SIGNALS:
  void checked(bool);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setChecked(bool);

public:
  explicit BoolBox(QWidget *parent = nullptr);
  bool isChecked();
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // BOOLBOX_UTILS_H
