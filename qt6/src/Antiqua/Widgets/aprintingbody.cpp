// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingbody.h"

#include <QPalette>

namespace AntiquaCRM {

APrintingBody::APrintingBody(QWidget *parent) : QTextEdit{parent} {
  setContentsMargins(0, 0, 0, 0);
  QPalette _palette = parentWidget()->palette();
  _palette.setColor(QPalette::Base, Qt::white);
  _palette.setColor(QPalette::Text, Qt::black);
  setPalette(_palette);
  setTextColor(Qt::black);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setStyleSheet("QTextEdit {color:black;border:none;}");
  const QString _css("* {color:black;} p,li {white-space:pre-wrap;}");
  document()->setDefaultStyleSheet(_css);
  document()->setModified(true);
}

const QTextCharFormat APrintingBody::charFormat(const QFont &font,
                                                bool bolded) {
  QTextCharFormat _format;
  QFont _font(font);
  _font.setBold(bolded);
  _format.setFont(_font);
  return _format;
}

const QTextTableFormat APrintingBody::tableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setMargin(0);
  _f.setBorder(0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  _f.setAlignment(Qt::AlignLeft | Qt::AlignTop);
  return _f;
}

const QTextTableCellFormat APrintingBody::cellFormat() {
  QTextTableCellFormat _f;
  _f.setBorder(0);
  _f.setPadding(0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  return _f;
}

const QTextBlockFormat APrintingBody::alignLeft() {
  QTextBlockFormat tbf;
  tbf.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  return tbf;
}

const QTextBlockFormat APrintingBody::alignRight() {
  QTextBlockFormat tbf;
  tbf.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return tbf;
}

const QTextBlockFormat APrintingBody::alignCenter() {
  QTextBlockFormat tbf;
  tbf.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return tbf;
}

} // namespace AntiquaCRM
