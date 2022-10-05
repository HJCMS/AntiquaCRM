// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_LIBRARY_H
#define BOOKLOOKER_LIBRARY_H

#include <QObject>

class Booklooker final : public QObject {
  Q_OBJECT

public:
  explicit Booklooker(QObject *parent = nullptr);
};

#endif
