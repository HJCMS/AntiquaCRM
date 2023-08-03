// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_APRINTINGBODY_H
#define ANTIQUACRM_PRINTING_APRINTINGBODY_H

#include <AGlobal>
#include <QFont>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextTableFormat>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY APrintingBody final : public QTextEdit {
  Q_OBJECT

public:
  explicit APrintingBody(QWidget *parent);

  /**
   * @brief General text format
   */
  const QTextCharFormat charFormat(const QFont &font, bool bolded = false);

  /**
   * @brief Default table format
   */
  const QTextTableFormat tableFormat();

  /**
   * @brief Default table cell format
   */
  const QTextTableCellFormat cellFormat();

  /**
   * @brief Text Block align left
   */
  const QTextBlockFormat alignLeft();

  /**
   * @brief Text Block align Center
   */
  const QTextBlockFormat alignCenter();

  /**
   * @brief Text Block align right
   */
  const QTextBlockFormat alignRight();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_APRINTINGBODY_H
