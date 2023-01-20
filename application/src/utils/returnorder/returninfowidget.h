// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURNINFOWIDGET_H
#define RETURNINFOWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class ButtonNext;

class InfoLabel final : public QLabel {
  Q_OBJECT

public:
  explicit InfoLabel(const QString &text, QWidget *parent = nullptr);
};

class ReturnInfoWidget : public QWidget {
  Q_OBJECT

private:
  ButtonNext *btn_next;

Q_SIGNALS:
  void sendConfirm();

public:
  explicit ReturnInfoWidget(QWidget *parent = nullptr);
};

#endif // RETURNINFOWIDGET_H