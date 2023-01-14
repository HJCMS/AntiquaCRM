// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SPLITTER_H
#define ANTIQUACRM_SPLITTER_H

#include <QObject>
#include <QSplitter>
#include <QWidget>

class AntiquaSplitter final : public QSplitter {
  Q_OBJECT

public:
  explicit AntiquaSplitter(QWidget *parent = nullptr);
  void addLeft(QWidget *widget);
  void addRight(QWidget *widget);
};

#endif // ANTIQUACRM_SPLITTER_H
