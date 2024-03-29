// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ABOUT_ANTIQUACRM_H
#define ANTIQUACRM_ABOUT_ANTIQUACRM_H

#include <QWidget>

// info
#include "infowidget.h"

/**
 * @ingroup _aboutdialogs
 * @brief About AntiquaCRM InfoWidget
 */
class AboutAntiquaCRM : public InfoWidget {
  Q_OBJECT

public:
  explicit AboutAntiquaCRM(QWidget *parent = nullptr);
  void setInfoWidget() override;
};

#endif // ANTIQUACRM_ABOUT_ANTIQUACRM_H
