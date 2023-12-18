// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_CONFIGWIDGET_H
#define ANTIQUACRM_STATISTICS_CONFIGWIDGET_H

#include <AntiquaWidgets>
#include <QFont>
#include <QLabel>
#include <QRegularExpression>
#include <QSignalMapper>

namespace AntiquaCRM {
class AWhatsThisButton;
};

class ANTIQUACRM_LIBRARY StatisticsConfigWidget final
    : public AntiquaCRM::PluginConfigWidget {
  Q_OBJECT

private:
  QFont chartbar_font;
  QSignalMapper *m_signalMapper;
  QLabel *section(const QString &suffix, const QString &title);
  QPushButton *button(const QString &objName);
  AntiquaCRM::AWhatsThisButton *fontTip(const QString &);

private Q_SLOTS:
  void setSectionFont(const QString &);
  void openFontDialog(QLabel *);

public Q_SLOTS:
  void loadSectionConfig() override;
  void saveSectionConfig() override;

public:
  explicit StatisticsConfigWidget(QWidget *parent = nullptr);
  AntiquaCRM::ConfigType getType() const override;
  const QJsonObject getMenuEntry() const override;
  static const QString fontPrefix();
  static const QRegularExpression fontPattern();
};

#endif // ANTIQUACRM_STATISTICS_CONFIGWIDGET_H
