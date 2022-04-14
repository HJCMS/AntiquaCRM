// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATADELEGATION_H
#define DATADELEGATION_H

#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

class DataDelegation : public QItemDelegate {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit DataDelegation(QTableView *parent = nullptr);
  virtual ~DataDelegation() {}
};

#endif // DATADELEGATION_H
