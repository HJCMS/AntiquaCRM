// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_LIBRARY_H
#define ABEBOOKS_LIBRARY_H

#include <QObject>

class Abebooks final : public QObject {
  Q_OBJECT

public:
  explicit Abebooks(QObject *parent = nullptr);
};

#endif
