// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_BOOKCARDPAGE_H
#define ANTIQUACRM_PRINTING_BOOKCARDPAGE_H

#include <AGlobal>
#include <QJsonObject>
#include <QPaintEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class BookCardPage
 * @brief Bookcard Painting Device
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY BookCardPage final : public QWidget {
  Q_OBJECT

private:
  const QJsonObject p_data;
  virtual void paintEvent(QPaintEvent *) override final;

public:
  explicit BookCardPage(const QJsonObject &data, QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_BOOKCARDPAGE_H
