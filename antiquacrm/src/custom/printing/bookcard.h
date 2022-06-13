// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_BOOKCARD_H
#define PRINTING_BOOKCARD_H

#include <QDialog>
#include <QFont>
#include <QMarginsF>
#include <QObject>
#include <QPageLayout>
#include <QPageSize>
#include <QPrinter>
#include <QUrl>
#include <QWidget>

// libQTQRcode
#include "qtqrcode.h"
#include "qtqrcodepainter.h"

class ApplSettings;
class TextEditor;

class BookCardPaintWidget final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_id;
  QString p_author;
  QString p_title;
  QString p_year;
  QString p_storage;
  QUrl p_queryUrl;
  const QSize qr_size = QSize(200, 200);
  QtQrCode m_qrCode;
  QtQrCodePainter m_qrCodePainter;

protected:
  void paintEvent(QPaintEvent *);

public:
  explicit BookCardPaintWidget(QWidget *parent);
  void setQrUrl(const QUrl &);
  void setArticleId(const QString &);
  void setStorage(const QString &);
  void setBookDescription(const QHash<QString, QVariant> &);
};

/**
 * @brief Buchkarte Erstellung
 * @ingroup Printing
 * @class BookCard
 */
class BookCard final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_printerName = QString();
  int p_articleId = -1;
  ApplSettings *config;
  QPageSize page_size;
  BookCardPaintWidget *m_card;
  QString p_filename;
  QString p_destination;

  void readConfiguration();

  const QUrl generateQRCodeUrl();

  const QPageLayout pageLayout();

  bool createPDF();

private Q_SLOTS:
  bool printDocument(QPrinter *printer);
  void openPrintDialog();

public:
  explicit BookCard(QWidget *paren = nullptr);
  int exec(const QHash<QString, QVariant> &data);
};

#endif // PRINTING_BOOKCARD_H
