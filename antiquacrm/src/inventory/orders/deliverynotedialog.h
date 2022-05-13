// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_DELIVERY_NOTE_H
#define INVENTORY_DELIVERY_NOTE_H

#include <QDialog>
#include <QObject>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QTextEdit>
#include <QWidget>

class PaintArea : public QTextEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTextDocument *m_document;
/*
protected:
  void paintEvent(QPaintEvent *event) override;
*/

public:
  explicit PaintArea(QWidget *parent = nullptr);
  void setDocumentHeader();
};

class DeliveryNoteDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  PaintArea *paintArea;
  QPainter painter;
  QPdfWriter *pdfwriter;

public:
  DeliveryNoteDialog(const QString &tmpfile, QWidget *parent = nullptr);
};

#endif // INVENTORY_DELIVERY_NOTE_H
