// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef YEAREDIT_H
#define YEAREDIT_H

#include <QtCore/QObject>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QWidget>

class YearEdit : public QDateEdit
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public Q_SLOTS:
  void setValue(const QVariant  &);

public:
  explicit YearEdit(QWidget *parent = nullptr);
  const QVariant value();

};

#endif // YEAREDIT_H

