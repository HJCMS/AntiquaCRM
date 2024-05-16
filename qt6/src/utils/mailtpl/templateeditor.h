// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_TEMPLATEEDITOR_H
#define ANTIQUACRM_UTILS_TEMPLATEEDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QWidget>

class TemplateEditor final : public QTextEdit {
  Q_OBJECT

private:
  void dragEnterEvent(QDragEnterEvent *) override;
  void dragMoveEvent(QDragMoveEvent *) override;

public Q_SLOTS:
  void setBody(const QString &data);

public:
  explicit TemplateEditor(QWidget *parent = nullptr);
  const QString getBody();
};

#endif // ANTIQUACRM_UTILS_TEMPLATEEDITOR_H
