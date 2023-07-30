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
#include <QTableWidget>
#include <QTableWidgetItem>

namespace AntiquaCRM {

/**
 * @class DeliveryNote
 * @brief Delivery note Painting Device
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY DeliveryNote final : public AntiquaCRM::APrintingPage {
  Q_OBJECT

private:
  QTableWidget *m_table;
  QFont normalFont;

  /**
   * @brief Textfield Start point!
   */
  const QPoint startPoint() const;

  /**
   * @brief Set Static Text indent ...
   */
  qreal indent = getPoints(5);

  /**
   * @brief Current Painter psotion.
   */
  mutable qreal position = -1;

  void setTableHeader(int column, const QString &title);
  void setData(int row, int column, const QString &title);

protected:
  void paintSubject(QPainter &painter);
  void paintContent(QPainter &painter) override;

public:
  explicit DeliveryNote(QWidget *parent = nullptr);
  bool setContentData(QJsonObject &data) override;
};

/**
 * @class PrintDeliveryNote
 * @brief Printing Delivery Notes Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintDeliveryNote : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  DeliveryNote *page;

private Q_SLOTS:
  void renderPage(QPrinter *printer) override;
  void createPDF() override;
  void openPrintDialog() override;

public:
  explicit PrintDeliveryNote(QWidget *parent = nullptr);
  int exec(const QJsonObject &options) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_DELIVERYNOTE_H
