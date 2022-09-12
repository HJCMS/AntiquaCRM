// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATADELEGATION_H
#define DATADELEGATION_H

#include <QObject>
#include <QPainter>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QTableView>
#include <QWidget>

class DataDelegation : public QItemDelegate {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit DataDelegation(QTableView *parent = nullptr);
  virtual ~DataDelegation() {}
};

#endif // DATADELEGATION_H
