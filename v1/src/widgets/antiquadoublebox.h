// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_DOUBLEBOX_H
#define ANTIQUA_INPUT_DOUBLEBOX_H

#include <QObject>
#include <QDoubleSpinBox>
#include <QWidget>

class AntiquaDoubleBox final : public QDoubleSpinBox {
  Q_OBJECT
  Q_PROPERTY(bool mouseWheel READ isMouseWheelEnabled WRITE setMouseWheelEnabled
                 NOTIFY sendMouseWheelChanged)

private:
  bool mouseWheel = false;

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