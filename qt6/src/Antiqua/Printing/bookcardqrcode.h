// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_BOOKCARDQRCODE_H
#define ANTIQUACRM_PRINTING_BOOKCARDQRCODE_H

#include <AGlobal>
#include <QImage>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @class BookCardQrCode
 * @brief create QR codes
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY BookCardQrCode final {

private:
  const QUrl p_url;
  const int p_size;

public:
  explicit BookCardQrCode(const QUrl &url, int size = 128);
  const QImage image();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_BOOKCARDQRCODE_H
