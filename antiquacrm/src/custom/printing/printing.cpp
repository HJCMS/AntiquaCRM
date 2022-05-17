// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printing.h"
#include "texteditor.h"

#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>

Printing::Printing(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(640, 580);

  headerFont = QFont("URW Chancery L [urw]", 26);
  normalFont = QFont("Tahoma", 10);
  smallFont = QFont("Tahoma", 8);
  // 210 x 297 mm, 8.26 x 11.69 inches
  page_margins = QMarginsF(5, 1, 1, 1);
  page_size = QPageSize(QPageSize::A4);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_layout");

  QRect pageRect = pageLayout().fullRectPoints();
  int maxWidth = pageRect.width();

  QScrollArea *scrollAera = new QScrollArea(this);
  scrollAera->setObjectName("scroll_area");
  layout->addWidget(scrollAera);

  printArea = new QWidget(scrollAera);
  printArea->setObjectName("printing_area");
  printArea->setContentsMargins(0, 0, 0, 0);
  printArea->setStyleSheet("border:none;");
  printArea->setFixedSize(pageRect.size());
  QVBoxLayout *frame_layout = new QVBoxLayout(printArea);
  frame_layout->setContentsMargins(0, 0, 0, 0);
  frame_layout->setSizeConstraint(QLayout::SetMinimumSize);
  frame_layout->setSpacing(0);

  header = new TextEditor(printArea);
  header->setObjectName("printing_header");
  header->setMinimumHeight(25);
  header->setMaximumWidth(maxWidth);
  header->setTextColor(Qt::black);
  header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(header);
  body = new TextEditor(printArea);
  body->setObjectName("printing_body");
  body->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
  body->setMaximumWidth(maxWidth);
  body->setTextColor(Qt::black);
  frame_layout->addWidget(body);
  footer = new TextEditor(printArea);
  footer->setObjectName("printing_footer");
  footer->setMinimumHeight(20);
  footer->setMaximumWidth(maxWidth);
  footer->setTextColor(Qt::black);
  footer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  frame_layout->addWidget(footer);
  frame_layout->setStretchFactor(header, 30);
  frame_layout->setStretchFactor(body, 40);
  frame_layout->setStretchFactor(footer, 20);
  scrollAera->setWidget(printArea);

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  printButton = buttonBox->addButton(tr("Printing"), /* Sofort drucken */
                                     QDialogButtonBox::ActionRole);
  layout->addWidget(buttonBox);

  setLayout(layout);

  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

const QTextCharFormat Printing::headerFormat() {
  QTextCharFormat f;
  f.setFont(headerFont);
  return f;
}

const QTextCharFormat Printing::normalFormat() {
  QTextCharFormat f;
  f.setFont(normalFont);
  return f;
}

const QTextCharFormat Printing::smallFormat() {
  QTextCharFormat f;
  f.setFont(smallFont);
  return f;
}

const QTextTableFormat Printing::tableFormat() {
  QTextTableFormat f;
  f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  f.setCellPadding(5);
  f.setCellSpacing(0);
  f.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  f.setTopMargin(5);
  return f;
}

const QPageSize Printing::pageSize() { return page_size; }

const QPageLayout Printing::pageLayout() {
  QPageLayout pageLayout;
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setPageSize(page_size, page_margins);
  pageLayout.setMode(QPageLayout::FullPageMode);
  pageLayout.setUnits(QPageLayout::Millimeter);
  return pageLayout;
}

bool Printing::fontFamilyExists(const QString &family) {
  foreach (QString f, QFontDatabase().families(QFontDatabase::Latin)) {
    if (f.contains(family, Qt::CaseSensitive)) {
      return true;
    }
  }
  return false;
}

void Printing::setHeaderFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    headerFont = font;
    emit headerFontChanged();
  }
}

const QFont Printing::getHeaderFont() { return headerFont; }

void Printing::setNormalFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    normalFont = font;
    emit normalFontChanged();
  }
}

const QFont Printing::getNormalFont() { return normalFont; }

void Printing::setSmallFont(const QFont &font) {
  if (fontFamilyExists(font.family())) {
    smallFont = font;
    emit smallFontChanged();
  }
}

const QFont Printing::getSmallFont() { return smallFont; }
