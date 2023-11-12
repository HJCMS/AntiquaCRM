// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_STEPTWO_H
#define ANTIQUACRM_REFUNDING_STEPTWO_H

#include "refundingpage.h"

class ANTIQUACRM_LIBRARY RefundStepTwo final : public RefundingPage {
  Q_OBJECT

public:
  explicit RefundStepTwo(QWidget *parent = nullptr);
  bool addDataRecord(qint64 id) override;
};

#endif // ANTIQUACRM_REFUNDING_STEPTWO_H
