// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_KEYWORDITEM_H
#define CATEGORYEDIT_KEYWORDITEM_H

#include <QTreeWidgetItem>

/**
 * @brief Baumelement für Schlüsselwörter
 * @ingroup CategoryEdit
 */
class TreeKeywordItem : public QTreeWidgetItem {
public:
  explicit TreeKeywordItem(QTreeWidgetItem *parent);
};

#endif // CATEGORYEDIT_KEYWORDITEM_H
