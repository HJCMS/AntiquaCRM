// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printing.h"
#include "texteditor.h"

#include <QDebug>
#include <QFontDatabase>
#include <QMarginsF>
#include <QScrollArea>
#include <QTextFrameFormat>
#include <QTextLength>
#include <QVBoxLayout>

Printing::Printing(QWidget *parent) : QDialog{parent} {
  setObjectName("printing_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(600, 400);

  headerFont = QFont("URW Chancery L [urw]", 26);
  normalFont = QFont("Tahoma", 11);
  smallFont = QFont("Tahoma", 8);
  page_size = QPageSize(QPageSize::A4);
  page_layout =
      QPageLayout(page_size, QPageLayout::Portrait, QMarginsF(0, 0, 0, 0),
                  QPageLayout::Point, QMarginsF(0, 0, 0, 0));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printing_layout");

  QScrollArea *scroll_area = new QScrollArea(this);
  scroll_area->setVisible(true);
  scroll_area->setWidgetResizable(true);
  layout->addWidget(scroll_area);

  editor = new TextEditor(scroll_area);
  editor->setObjectName("printing_editor");
  editor->setBaseSize(page_size.sizePoints());
  scroll_area->setWidget(editor);

  buttonBox = new QDialogButtonBox(
      (QDialogButtonBox::Close | QDialogButtonBox::Apply), this);
  printButton =
      buttonBox->addButton(tr("Printing"), QDialogButtonBox::ActionRole);
  layout->addWidget(buttonBox);

  setLayout(layout);
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

const QPageLayout Printing::pageLayout() { return page_layout; }

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
