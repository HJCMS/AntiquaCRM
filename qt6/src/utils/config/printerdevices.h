// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERDEVICES_H
#define ANTIQUACRM_DIALOGS_PRINTERDEVICES_H

#include <AntiquaWidgets>
#include <QGroupBox>
#include <QLabel>

class PrinterDevices final : public QGroupBox {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_mainPrinter;
  AntiquaCRM::AComboBox *m_slavePrinter;

  QLabel *label(const QString &);

  const QIcon icon() const;

private Q_SLOTS:
  void setDeviceChanged(int);

public Q_SLOTS:
  void loadSection(AntiquaCRM::ASettings *);
  void saveSection(AntiquaCRM::ASettings *);

public:
  explicit PrinterDevices(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_PRINTERDEVICES_H
