// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RECKONING_PRINTING_H
#define RECKONING_PRINTING_H

#include <QObject>
#include <QWidget>

#include "printing.h"

/**
 * @brief Rechnungs Erstellung
 * @ingroup Printing
 * @class Reckoning
 */
class Reckoning : public Printing {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  void constructHeader(){/* TODO */};
  void constructBody(){/* TODO */};
  void constructFooter(){/* TODO */};

private Q_SLOTS:
  void printToFile(QPrinter *printer){/* TODO */};
  void printDocument(QPrinter *printer){/* TODO */};
  void openPrintDialog(){/* TODO */};

public:
  explicit Reckoning(QWidget *parent = nullptr);
};

#endif // RECKONING_PRINTING_H
