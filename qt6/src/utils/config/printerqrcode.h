// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERQRCODE_H
#define ANTIQUACRM_DIALOGS_PRINTERQRCODE_H

#include <AntiquaWidgets>
#include <QGroupBox>
#include <QLabel>

class PrinterQRCode final : public QGroupBox {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_url;
  AntiquaCRM::ALineEdit *m_query;
  QLabel *label(const QString &title);

public Q_SLOTS:
  void loadSection(AntiquaCRM::ASettings *);
  void saveSection(AntiquaCRM::ASettings *);

public:
  explicit PrinterQRCode(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_PRINTERQRCODE_H
