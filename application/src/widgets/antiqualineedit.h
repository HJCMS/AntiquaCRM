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

/**
 * @brief The AntiquaLineEdit class
 * @ingroup widgets
 */
class AntiquaLineEdit final : public QLineEdit {
  Q_OBJECT
  Q_PROPERTY(bool keyReturn READ keyEnterEventEnabled WRITE setEnableKeyEnterEvent
             NOTIFY sendKeyEnterEventChanged);

private:
  bool keyEnterEvent;

protected:
  void focusInEvent(QFocusEvent *event);

protected Q_SLOTS:
  virtual void checkEnterEventPressed();

Q_SIGNALS:
  void sendKeyEnterEventChanged();

public:
  explicit AntiquaLineEdit(QWidget *parent);
  void setEnableKeyEnterEvent(bool);
  bool keyEnterEventEnabled();
};

#endif // ANTIQUA_INPUT_LINEEDIT_H
