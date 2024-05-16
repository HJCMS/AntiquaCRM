// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERPAPERLAYOUT_H
#define ANTIQUACRM_DIALOGS_PRINTERPAPERLAYOUT_H

#include <AntiquaWidgets>
#include <QGroupBox>
#include <QLabel>

class PrinterSetBorder;

class PrinterPaperLayout final : public QGroupBox {
  Q_OBJECT

private:
  PrinterSetBorder *m_marginLeft;
  PrinterSetBorder *m_marginRight;
  PrinterSetBorder *m_marginTop;
  PrinterSetBorder *m_marginBottom;

  QLabel *label(const QString &title);

private Q_SLOTS:
  void setInputChanges(bool);

public Q_SLOTS:
  void loadSection(AntiquaCRM::ASettings *);
  void saveSection(AntiquaCRM::ASettings *);

public:
  explicit PrinterPaperLayout(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_PRINTERPAPERLAYOUT_H
