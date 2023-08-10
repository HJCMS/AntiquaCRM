// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_BOOKCARD_H
#define ANTIQUACRM_PRINTING_BOOKCARD_H

#include <AntiquaPrintDialog>
#include <QImage>
#include <QObject>
#include <QUrl>
#include <QWidget>

namespace AntiquaCRM {

class BookCardPage;

/**
 * @class PrintBookCard
 * @brief Printing Bookcard Dialog
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY PrintBookCard final : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  /**
   * @brief Render page, initialed in exec function.
   */
  BookCardPage *page = nullptr;

  bool notValid(const QJsonValue &) const;

private Q_SLOTS:
  /**
   * @brief Render BookCardPage to Printer Device
   */
  void renderPage(QPrinter *printer) override;

  /**
   * @brief Create PDF from Rendering page ...
   */
  void createPDF() override;

  /**
   * @brief Initial/config and open QPrintDialog ...
   */
  void openPrintDialog() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit PrintBookCard(QWidget *parent = nullptr);

  /**
   * @brief Open Book card printin dialog
   * @param opts - Options
   * @code
   * QJsonObject({
   *    "aid" : qint64,          // Article ID
   *    "author" : QString,      // Authors
   *    "basename" : QString,    // File basename
   *    "category" : QString,    // Stored in category
   *    "changed" : QString,     // formated Date string
   *    "compartment" : QString, // Storage compartment
   *    "name" : QString,        // Name
   *    "qrquery" : QString,     // QUrl Query String
   *    "storage" : Qint64,      // Storage Id from SQL database
   *    "title" : QString,       // Title
   *    "year" : QString         // Year String format: yyyy
   * })
   * @endcode
   */
  int exec(const QJsonObject &opts) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_BOOKCARD_H
