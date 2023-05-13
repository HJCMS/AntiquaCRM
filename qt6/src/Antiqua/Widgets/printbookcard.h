// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_BOOKCARD_H
#define ANTIQUACRM_WIDGETS_PRINT_BOOKCARD_H

#include <AntiquaPrintDialog>
#include <QImage>
#include <QObject>
#include <QUrl>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @brief QRCode erstellen
 * @ingroup _printing
 */
class BookCardQrCode final {

private:
  const QUrl p_url;
  const int p_size;

public:
  explicit BookCardQrCode(const QUrl &url, int size = 128);
  const QImage image();
};

/**
 * @class BookCardPage
 * @brief Bookcard Painting Device
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY BookCardPage final : public QWidget {
  Q_OBJECT

private:
  const QJsonObject p_data;
  void paintEvent(QPaintEvent *) override;

public:
  explicit BookCardPage(const QJsonObject &data, QWidget *parent = nullptr);
};

/**
 * @class PrintBookCard
 * @brief Printing Bookcard Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintBookCard final : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  BookCardPage *page;

private Q_SLOTS:
  void renderPage(QPrinter *printer) override;
  void createPDF() override;
  void openPrintDialog() override;

public:
  explicit PrintBookCard(QWidget *parent = nullptr);
  int exec(const QJsonObject &options) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_BOOKCARD_H
