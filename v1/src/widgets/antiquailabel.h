// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_ILABEL_H
#define ANTIQUA_INPUT_ILABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class AntiquaILabel final : public QLabel {
  Q_OBJECT

public:
  explicit AntiquaILabel(QWidget *parent = nullptr);
};

#endif // ANTIQUA_INPUT_ILABEL_H
