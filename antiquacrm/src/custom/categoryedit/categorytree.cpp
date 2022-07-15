// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorytree.h"

#include <QtCore>
#include <QtWidgets>

CategoryTree::CategoryTree(QWidget *parent) : QTreeWidget{parent}
{
  setObjectName("category_tree");
}
