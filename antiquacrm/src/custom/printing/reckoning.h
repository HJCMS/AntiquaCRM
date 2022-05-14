// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RECKONING_PRINTING_H
#define RECKONING_PRINTING_H

#include <QObject>
#include <QWidget>

#include "printing.h"

class Reckoning : public Printing
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  void constructHeader() { /* TODO */ };
  void constructBody() { /* TODO */ };
  void constructFooter() { /* TODO */ };

private Q_SLOTS:

Q_SIGNALS:

public:
  explicit Reckoning(QWidget *parent = nullptr);

};

#endif // RECKONING_PRINTING_H
