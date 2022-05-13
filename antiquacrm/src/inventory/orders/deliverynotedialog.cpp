// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverynotedialog.h"

//#include <QPageSize>
//#include <QPageLayout>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// https://code-examples.net/en/q/201852c
PaintArea::PaintArea(QWidget *parent) : QTextEdit{parent} {
  setTextColor(QColor(0, 0, 0));
  setAcceptRichText(true);
  m_document = document();
}

// void PaintArea::paintEvent(QPaintEvent *event) {}

void PaintArea::setDocumentHeader() {
  QTextBlock block = m_document->firstBlock();
  QTextFormat format;
  QTextCursor tc = textCursor();
  tc.beginEditBlock();
  tc.insertText("Buch- und Kunsthandlung Antiquariat");
  tc.endEditBlock();
}

DeliveryNoteDialog::DeliveryNoteDialog(const QString &tmpfile, QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(800, 600);

  QVBoxLayout *layout = new QVBoxLayout(this);
  /*
    QPageLayout page(QPageSize(QPageSize::A4), QPageLayout::Portrait,
                     QMargins(30, 30, 30, 30), QPageLayout::Millimeter);
    pdfwriter = new QPdfWriter(tmpfile);
    pdfwriter->setPageLayout(page);
    pdfwriter->setResolution(300);
  */
  paintArea = new PaintArea(this);
  paintArea->setStyleSheet("background: white; color: black;");
  paintArea->setDocumentHeader();
  layout->addWidget(paintArea);

  QDialogButtonBox *box = new QDialogButtonBox(
      (QDialogButtonBox::Close | QDialogButtonBox::Save), this);
  layout->addWidget(box);

  setLayout(layout);

  connect(box, SIGNAL(rejected()), this, SLOT(reject()));
  connect(box, SIGNAL(accepted()), this, SLOT(accept()));
}
