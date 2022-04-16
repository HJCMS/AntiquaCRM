// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTSEDITOR_H
#define PRINTSEDITOR_H

#include <QObject>
#include <QWidget>

class PrintsEditor : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

Q_SIGNALS:

public:
  explicit PrintsEditor(QWidget *parent = nullptr);

};

#endif // PRINTSEDITOR_H
