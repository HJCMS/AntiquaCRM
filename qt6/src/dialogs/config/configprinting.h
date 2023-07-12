// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGPRINTING_H
#define ANTIQUACRM_DIALOGS_CONFIGPRINTING_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QEvent>
#include <QObject>

class PrinterDevices;
class PrinterFonts;
class PrinterAttachments;
class PrinterPaperLayout;
class PrinterQRCode;

class ConfigPrinting final : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  PrinterDevices *m_printers;
  PrinterFonts *m_fontGroup;
  PrinterAttachments *m_attachmentGroup;
  PrinterPaperLayout *m_paperLayout;
  PrinterQRCode *m_qrCode;

  /**
   * @brief find edit changes
   */
  bool eventFilter(QObject *, QEvent *) override;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigPrinting(QWidget *parent = nullptr);
  AntiquaCRM::PluginConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGPRINTING_H
