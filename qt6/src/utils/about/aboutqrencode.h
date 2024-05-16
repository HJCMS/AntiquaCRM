// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTQRENCODE_H
#define ANTIQUACRM_UTILS_ABOUTQRENCODE_H

#include "infowidget.h"

class AboutQREncode final : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutQREncode(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_ABOUTQRENCODE_H
