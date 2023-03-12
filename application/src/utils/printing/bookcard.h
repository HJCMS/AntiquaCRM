// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PRINTING_BOOKCARD_H
#define PRINTING_BOOKCARD_H

#include <AntiquaCRM>
#include <QComboBox>
#include <QDialog>
#include <QFont>
#include <QImage>
#include <QLabel>
#include <QMarginsF>
#include <QObject>
#include <QPageLayout>
#include <QPageSize>
#include <QPrinter>
#include <QRunnable>
#include <QUrl>
#include <QWidget>

extern "C" {
#include "qrencode.h"
};

class TextEditor;

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
 * @brief Datenfelder erstellen
 * @ingroup _printing
 */
class BookCardPaintWidget final : public QWidget {
  Q_OBJECT

private:
  QString p_id;
  QString p_author;
  QString p_title;
  QString p_year;
  QString p_storage;
  QString p_since;
  QString p_keywords;
  QUrl p_queryUrl;

protected:
  void paintEvent(QPaintEvent *);

public:
  explicit BookCardPaintWidget(QWidget *parent);
  void setQrUrl(const QUrl &);
  void setArticleId(const QString &);
  void setStorage(int id);
  void setBookDescription(const QHash<QString, QVariant> &);
};

/**
 * @brief Buchkarten Konfiguration
 * @ingroup _printing
 */
class BookCardConfig final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;
  QComboBox *m_printer;
  QComboBox *m_printerPaperFeed;

private Q_SLOTS:
  void infoPrinterChanged(int);
  void paperSourceChanged(int);

Q_SIGNALS:
  void s_printerChanged(const QString &);
  void s_paperSourceChanged(QPrinter::PaperSource);

public Q_SLOTS:
  void setPrinterByName(const QString &);
  void printerChanged(QPrinter *);

public:
  BookCardConfig(QWidget *parent = nullptr);
};

/**
 * @ingroup _printing
 * @brief Printing Article Bookcards
 */
class BookCard final : public QDialog {
  Q_OBJECT

private:
  QString p_printerName = QString();
  int p_articleId = -1;
  AntiquaCRM::ASettings *config;
  BookCardPaintWidget *m_card;
  BookCardConfig *m_cardConfig;
  QString p_filename;
  QString p_destination;
  QPrinter::PaperSource p_paperSource;

  void readConfiguration();

  const QUrl generateQRCodeUrl();

  const QPageLayout pageLayout();

  bool createPDF();

private Q_SLOTS:
  void updatePrinterByName(const QString &);
  void updatePaperSource(QPrinter::PaperSource);
  bool printDocument(QPrinter *printer);
  void openPrintDialog();

public:
  explicit BookCard(QWidget *paren = nullptr);
  int exec() override;
  int exec(const QHash<QString, QVariant> &data);
};

#endif // PRINTING_BOOKCARD_H
