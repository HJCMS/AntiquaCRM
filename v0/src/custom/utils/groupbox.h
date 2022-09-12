// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GROUPBOX_UTILS_H
#define GROUPBOX_UTILS_H

#include <QObject>
#include <QLabel>
#include <QGroupBox>
#include <QWidget>

#include <UtilsMain>

class GroupBox : public UtilsMain
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  QGroupBox *box;
  explicit GroupBox(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // GROUPBOX_UTILS_H

