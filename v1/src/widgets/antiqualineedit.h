// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_LINEEDIT_H
#define ANTIQUA_INPUT_LINEEDIT_H

#include <QCompleter>
#include <QFocusEvent>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

class AntiquaLineEdit final : public QLineEdit {
  Q_OBJECT

protected:
  void focusInEvent(QFocusEvent *event);

public:
  explicit AntiquaLineEdit(QWidget *parent);
};

#endif // ANTIQUA_INPUT_LINEEDIT_H
