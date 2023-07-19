// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_COMPANYWIDGET_H
#define ANTIQUACRM_UTILS_COMPANYWIDGET_H

#include <AntiquaWidgets>
#include <QGridLayout>
#include <QLabel>

class CompanyWidget final : public QWidget {
  Q_OBJECT

private:
  int p_row = 0; /**< @brief layout row */
  QGridLayout *layout;
  QLabel *infoCell(const QString &title);

Q_SIGNALS:
  void sendSaved(bool);

public Q_SLOTS:
  void saveConfig();

public:
  explicit CompanyWidget(QWidget *parent = nullptr);
  bool loadConfig();
};

#endif // ANTIQUACRM_UTILS_COMPANYWIDGET_H
