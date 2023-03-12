// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef RETURNINFOWIDGET_H
#define RETURNINFOWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class ButtonNext;

/**
 * @ingroup _returnorder
 * @brief The InfoLabel class
 */
class InfoLabel final : public QLabel {
  Q_OBJECT

public:
  explicit InfoLabel(const QString &text, QWidget *parent = nullptr);
};

/**
 * @brief The ReturnInfoWidget class
 * @ingroup _returnorder
 */
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
