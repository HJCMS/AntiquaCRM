// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_H
#define CATEGORYEDIT_H

#include <QObject>
#include <QDialog>
#include <QWidget>

class CategoryEdit : public QDialog
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

public:
  explicit CategoryEdit(QWidget *parent = nullptr);

};

#endif // CATEGORYEDIT_H
