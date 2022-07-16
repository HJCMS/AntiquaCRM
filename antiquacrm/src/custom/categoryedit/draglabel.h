// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_DRAGLABEL_H
#define CATEGORYEDIT_DRAGLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class DragLabel : public QLabel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit DragLabel(const QString &text, QWidget *parent = nullptr);
};

#endif // CATEGORYEDIT_DRAGLABEL_H
