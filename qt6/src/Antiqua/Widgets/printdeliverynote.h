// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_DELIVERYNOTE_H
#define ANTIQUACRM_WIDGETS_PRINT_DELIVERYNOTE_H

#include <AntiquaPrintDialog>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PrintDeliveryNote
 * @brief Printing Delivery Notes Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintDeliveryNote : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  bool generateDocument(QPrinter *printer) override;

private Q_SLOTS:
  void openPrintDialog() override;

public:
  explicit PrintDeliveryNote(QWidget *parent = nullptr);
  int exec(const QStringList &options) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_DELIVERYNOTE_H
