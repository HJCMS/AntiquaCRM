// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_TREE_H
#define CATEGORYEDIT_TREE_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>

class CategoryTree : public QTreeWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit CategoryTree(QWidget *parent = nullptr);

};

#endif // CATEGORYEDIT_TREE_H
