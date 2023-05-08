// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERSETBORDER_H
#define ANTIQUACRM_DIALOGS_PRINTERSETBORDER_H

#include <AntiquaWidgets>
#include <QFrame>

class PrinterSetBorder final : public QFrame {
  Q_OBJECT

private:
  AntiquaCRM::ASpinBox *m_value;
  AntiquaCRM::ASlider *m_slider;

private Q_SLOTS:
  void sliderChanged(int);

public Q_SLOTS:
  void setValue(int);

public:
  explicit PrinterSetBorder(const QString &info, QWidget *parent = nullptr);
  double getValue();
};

#endif // PRINTERSETBORDER_H
