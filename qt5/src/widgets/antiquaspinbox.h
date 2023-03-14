// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_INPUT_SPINBOX_H
#define ANTIQUA_INPUT_SPINBOX_H

#include <QObject>
#include <QSpinBox>
#include <QWidget>

/**
 * @brief The AntiquaSpinBox class
 * @ingroup widgets
 */
class AntiquaSpinBox final : public QSpinBox {
  Q_OBJECT
  Q_PROPERTY(bool mouseWheel READ isMouseWheelEnabled WRITE setMouseWheelEnabled
                 NOTIFY sendMouseWheelChanged)

private:
  bool mouseWheel;

protected:
  void wheelEvent(QWheelEvent *e) override;
  void setMouseWheelEnabled(bool);

Q_SIGNALS:
  void sendMouseWheelChanged();

public:
  explicit AntiquaSpinBox(QWidget *parent = nullptr);
  bool isMouseWheelEnabled();
};

#endif // ANTIQUA_INPUT_SPINBOX_H
