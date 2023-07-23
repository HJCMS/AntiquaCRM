// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H
#define ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H

#include "infowidget.h"

class AboutAntiquaCRM final : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutAntiquaCRM(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_ABOUTANTIQUACRM_H
