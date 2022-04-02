// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMBOFIELDBOX_H
#define COMBOFIELDBOX_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QComboBox>

class ComboFieldBox : public QComboBox {
  Q_OBJECT

public Q_SLOTS:
  void setIndex(int);
  void setIndex(const QString &);

public:
  explicit ComboFieldBox(QWidget *parent = nullptr);
};

#endif // COMBOFIELDBOX_H
