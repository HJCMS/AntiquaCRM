// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_PRINTERFONTS_H
#define ANTIQUACRM_DIALOGS_PRINTERFONTS_H

#include <AntiquaWidgets>
#include <QGroupBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QSignalMapper>

namespace AntiquaCRM {
class AWhatsThisButton;
};

class PrinterFonts final : public QGroupBox {
  Q_OBJECT

private:
  QSignalMapper *m_signalMapper;
  const QString p_font_prefix = QString("print_font_");
  QLabel *section(const QString &suffix, const QString &title);
  QPushButton *button(const QString &objName);
  AntiquaCRM::AWhatsThisButton *fontTip(const QString &);

private Q_SLOTS:
  void updateSection(QLabel *);
  void setSectionFont(const QString &);

public Q_SLOTS:
  void loadSection(AntiquaCRM::ASettings *);
  void saveSection(AntiquaCRM::ASettings *);

public:
  explicit PrinterFonts(QWidget *parent = nullptr);
  static const QRegularExpression fontPattern();
};

#endif // ANTIQUACRM_DIALOGS_PRINTERFONTS_H
