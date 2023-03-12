// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef TEXTEDITOR_PRINTING_H
#define TEXTEDITOR_PRINTING_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @ingroup _printing
 * @brief Textbodies
 */
class TextEditor : public QTextEdit
{
  Q_OBJECT

public:
  explicit TextEditor(QWidget *parent = nullptr);

};

#endif // TEXTEDITOR_PRINTING_H
