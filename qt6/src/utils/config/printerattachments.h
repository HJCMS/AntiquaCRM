// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERATTACHMENTS_H
#define ANTIQUACRM_DIALOGS_PRINTERATTACHMENTS_H

#include <AntiquaWidgets>
#include <QGroupBox>
#include <QWidget>

class PrinterAttachments final : public QGroupBox {
  Q_OBJECT

private:
  AntiquaCRM::SelectTargets *m_path;
  AntiquaCRM::SelectFile *m_watermark;
  AntiquaCRM::ALabel *m_opacityLabel;
  AntiquaCRM::ASlider *m_opacity;

private Q_SLOTS:
  void subWidgetChanged();
  void opacityChanged(int);

public Q_SLOTS:
  void loadSection(AntiquaCRM::ASettings *);
  void saveSection(AntiquaCRM::ASettings *);

public:
  explicit PrinterAttachments(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_PRINTERATTACHMENTS_H
