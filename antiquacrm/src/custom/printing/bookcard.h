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
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QWidget>

class ApplSettings;
class TextEditor;

class BookCardPaintWidget final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_id;
  QStringList p_description;
  QString p_storage;

protected:
  void paintEvent(QPaintEvent *);

public:
  explicit BookCardPaintWidget(QWidget * parent);
  void setArticleId(const QString &);
  void setStorage(const QString &);
  void setBookDescription(const QStringList &);
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
  ApplSettings *config;
  QPageSize page_size;
  BookCardPaintWidget *m_card;
  QString p_filename;
  QString p_destination;

  void readConfiguration();

  const QPageLayout pageLayout();

private Q_SLOTS:
  void printDocument(QPrinter *printer);
  void openPrintDialog();

public:
  explicit BookCard(QWidget *paren = nullptr);
  int exec(const QHash<QString, QVariant> &data);
};

#endif // PRINTING_BOOKCARD_H
