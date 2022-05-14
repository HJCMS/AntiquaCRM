// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEXTEDITOR_PRINTING_H
#define TEXTEDITOR_PRINTING_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class TextEditor : public QTextEdit
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

private Q_SLOTS:

Q_SIGNALS:

public:
  explicit TextEditor(QWidget *parent = nullptr);

};

#endif // TEXTEDITOR_PRINTING_H
