// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef TEMPLATES_MESSAGEEDITOR_H
#define TEMPLATES_MESSAGEEDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @ingroup _templates
 * @brief Body Template Edit widget
 */
class MessageEditor : public QTextEdit {
  Q_OBJECT

public Q_SLOTS:
  void setBody(const QString &);

public:
  explicit MessageEditor(QWidget *parent = nullptr);
};

#endif // MESSAGEEDITOR_UTILS_H
