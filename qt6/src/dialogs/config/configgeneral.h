// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGGENERAL_H
#define ANTIQUACRM_DIALOGS_CONFIGGENERAL_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ConfigGeneral final : public AntiquaCRM::TabsConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::CurrencySelector *m_currency;
  AntiquaCRM::NumEdit *m_vatNormal;
  AntiquaCRM::NumEdit *m_vatReduced;

  void initUi();

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ConfigGeneral(QWidget *parent = nullptr);
  AntiquaCRM::TabsConfigWidget::ConfigType getType() const override;
  const QIcon getIcon() const override;
  const QString getTitle() const override;
};

#endif // ANTIQUACRM_DIALOGS_CONFIGGENERAL_H
