// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_ILABEL_H
#define ANTIQUA_INPUT_ILABEL_H

#include <QLabel>
#include <QObject>
#include <QVariant>
#include <QWidget>

class AntiquaILabel final : public QLabel {
  Q_OBJECT

public:
  explicit AntiquaILabel(QWidget *parent = nullptr);
  explicit AntiquaILabel(const QString &text, QWidget *parent = nullptr);
  void setValue(const QVariant &value);
};

#endif // ANTIQUA_INPUT_ILABEL_H
