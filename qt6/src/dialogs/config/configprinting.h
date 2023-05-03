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
#include <QObject>

class PrinterFonts;
class PrinterAttachments;

class ConfigPrinting final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
  PrinterFonts *m_fontGroup;
  PrinterAttachments *m_attachmentGroup;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigPrinting(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QIcon getIcon() const override;
  const QString getTitle() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGPRINTING_H
