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
#include <QBrush>
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

private:
  /**
   * @brief Brush for visible Borders
   */
  inline const QBrush borderBrush() const;

public:
  explicit APrintingBody(QWidget *parent);

  /**
   * @brief Default table format
   */
  const QTextTableFormat tableFormat();

  /**
   * @brief Default Table Cell format class
   */
  const QTextTableCellFormat tableCellFormat() const;

  /**
   * @brief Default table cell formats
   */
  const QTextTableCellFormat headerTableCellFormat();

  /**
   * @brief Default table article cell formats
   */
  const QTextTableCellFormat articleTableCellFormat(bool border = false);

  /**
   * @brief Text Block format
   */
  const QTextBlockFormat blockFormat(Qt::Alignment align = Qt::AlignLeft) const;

  /**
   * @brief Text Block align left valign middle
   */
  const QTextBlockFormat alignLeft();

  /**
   * @brief Text Block align Center valign middle
   */
  const QTextBlockFormat alignCenter();

  /**
   * @brief Text Block align right valign middle
   */
  const QTextBlockFormat alignRight();

  void setCellItem(QTextTableCell &cell, const QVariant &value,
                   Qt::Alignment align = (Qt::AlignCenter | Qt::AlignVCenter));
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_APRINTINGBODY_H
