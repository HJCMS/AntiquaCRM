// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H
#define ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H

#include <QWidget>

#include "infowidget.h"

class AboutAntiquaCRM : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutAntiquaCRM(QWidget *parent = nullptr);
  void setInfoWidget() override;
};

#endif // ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H
