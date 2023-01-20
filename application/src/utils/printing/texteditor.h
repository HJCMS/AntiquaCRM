// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEXTEDITOR_PRINTING_H
#define TEXTEDITOR_PRINTING_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @brief Textköper
 * @ingroup Printing
 * @class TextEditor
 */
class TextEditor : public QTextEdit
{
  Q_OBJECT

public:
  explicit TextEditor(QWidget *parent = nullptr);

};

#endif // TEXTEDITOR_PRINTING_H
