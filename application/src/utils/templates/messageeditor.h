// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_MESSAGEEDITOR_H
#define TEMPLATES_MESSAGEEDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @brief Body Template Edit widget
 * @class MessageEditor
 * @section widgets
 */
class MessageEditor : public QTextEdit {
  Q_OBJECT

public Q_SLOTS:
  void setBody(const QString &);

public:
  explicit MessageEditor(QWidget *parent = nullptr);
};

#endif // MESSAGEEDITOR_UTILS_H