// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURN_BUTTON_NEXT_H
#define RETURN_BUTTON_NEXT_H

#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @ingroup _returnorder
 * @brief The ButtonNext class
 */
class ButtonNext : public QFrame {
  Q_OBJECT

private:
  QPushButton *m_btn;

Q_SIGNALS:
  void clicked();

public:
  explicit ButtonNext(QWidget *parent = nullptr);
};

#endif // RETURN_BUTTON_NEXT_H
