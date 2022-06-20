// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTING_BOOKCARD_H
#define PRINTING_BOOKCARD_H

#include <QDialog>
#include <QFont>
#include <QImage>
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

class ApplSettings;
class TextEditor;

class BookCardQrCode final {

private:
  const int p_size;
  const QUrl p_url;

public:
  explicit BookCardQrCode(const QUrl &url, int size = 128);
  const QImage image();
};

/**
 * @brief Datenfelder einfügen
 */
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
 * @brief Buchkarten Erstellung
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
