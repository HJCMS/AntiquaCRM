// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_INPUT_DOUBLEBOX_H
#define ANTIQUA_INPUT_DOUBLEBOX_H

#include <QObject>
#include <QDoubleSpinBox>
#include <QWidget>

/**
 * @brief The AntiquaDoubleBox class
 * @ingroup widgets
 */
class AntiquaDoubleBox final : public QDoubleSpinBox {
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
  explicit AntiquaDoubleBox(QWidget *parent = nullptr);
  bool isMouseWheelEnabled();
};

#endif // ANTIQUA_INPUT_DOUBLEBOX_H
