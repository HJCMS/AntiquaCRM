// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef YEAREDIT_H
#define YEAREDIT_H

#include <QtCore/QObject>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QWidget>

class YearEdit : public QDateEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool modified;

public Q_SLOTS:
  void setModified(bool b = true);
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit YearEdit(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
};

#endif // YEAREDIT_H
