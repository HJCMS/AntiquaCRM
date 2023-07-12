// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLCONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_CDSVINYLCONFIGWIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>

class ANTIQUACRM_LIBRARY CDsVinylConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::PriceEdit *m_minPrice;
  AntiquaCRM::PriceEdit *m_normalPrice;
  AntiquaCRM::SelectStorage *m_storage;

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit CDsVinylConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::PluginConfigWidget::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLCONFIGWIDGET_H
