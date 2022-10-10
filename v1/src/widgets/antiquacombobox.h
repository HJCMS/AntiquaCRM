// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_COMBOBOX_H
#define ANTIQUA_INPUT_COMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QWidget>


class AntiquaComboBox final : public QComboBox {
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
  explicit AntiquaComboBox(QWidget *parent = nullptr);
  bool isMouseWheelEnabled();
};

#endif // ANTIQUACRM_COMBOBOX_H
