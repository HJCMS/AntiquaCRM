// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_VIEWS_CONFIGWIDGET_H
#define ANTIQUACRM_VIEWS_CONFIGWIDGET_H

#include <AntiquaWidgets>
#include <QFont>
#include <QLabel>
#include <QRegularExpression>
#include <QSignalMapper>

namespace AntiquaCRM {
class AWhatsThisButton;
};

class ANTIQUACRM_LIBRARY ViewsConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  AntiquaCRM::AWhatsThisButton *tip(const QString &);

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit ViewsConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
};

#endif // ANTIQUACRM_VIEWS_CONFIGWIDGET_H
