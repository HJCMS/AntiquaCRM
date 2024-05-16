// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_ADMONITION_H
#define ANTIQUACRM_PRINTING_ADMONITION_H

#include <AntiquaPrintDialog>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class AdmonitionNote
 * @brief Admonition note Painting Device
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY AdmonitionNote final
    : public AntiquaCRM::APrintingPage {
  Q_OBJECT

private:
  QJsonObject config;
  QTextTable *m_articles = nullptr;
  double summary = 0.00;
  void paintContent(QPainter &painter) override;
  virtual void setBodyLayout() override;
  void setArticleCell(int row, const QVariant &value);
  void setDescripeCell(int row, const QVariant &value);
  void setQuantityCell(int row, const QVariant &value);
  int addArticleRows(int row, const QSqlQuery &result);

public:
  explicit AdmonitionNote(QWidget *parent = nullptr);
  bool setContentData(QJsonObject &data) override;
};

/**
 * @class PrintAdmonition
 * @brief Printing Admonition Dialog
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintAdmonition final
    : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  AdmonitionNote *page;

private Q_SLOTS:
  void renderPage(QPrinter *printer) override;
  void createPDF() override;
  void openPrintDialog() override;

public:
  explicit PrintAdmonition(QWidget *parent = nullptr);
  int exec(const QJsonObject &options, bool pdfbtn = true) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_ADMONITION_H
