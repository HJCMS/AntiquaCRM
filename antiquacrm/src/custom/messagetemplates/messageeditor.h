// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGEEDITOR_UTILS_H
#define MESSAGEEDITOR_UTILS_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class MessageEditor : public QTextEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit MessageEditor(QWidget *parent = nullptr);
};

#endif // MESSAGEEDITOR_UTILS_H
