// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingbody.h"

#include <QChar>
#include <QMetaType>
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

const QBrush APrintingBody::borderBrush() const {
  return QBrush(Qt::gray, Qt::SolidPattern);
}

const QString APrintingBody::carriageReturn() const {
  QString _str;
  _str.append(QChar::CarriageReturn);
  return _str;
}

const QTextTableFormat APrintingBody::tableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setMargin(0);
  _f.setTopMargin(p_margin);
  _f.setBottomMargin(p_margin);
  _f.setBorder(0);
  _f.setAlignment(Qt::AlignLeft | Qt::AlignTop);
  return _f;
}

const QTextTableCellFormat APrintingBody::tableCellFormat() const {
  QTextTableCellFormat _f;
  _f.setPadding(0);
  _f.setBorder(0);
  _f.setPadding(1);
  return _f;
}

const QTextTableCellFormat APrintingBody::headerTableCellFormat() {
  QTextTableCellFormat _f = tableCellFormat();
  _f.setTopBorder(1);
  _f.setTopBorderBrush(borderBrush());
  _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  _f.setBottomBorder(1);
  _f.setBottomBorderBrush(borderBrush());
  _f.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  return _f;
}

const QTextTableCellFormat APrintingBody::articleTableCellFormat(bool border) {
  QTextTableCellFormat _f = tableCellFormat();
  if (border) {
    _f.setTopBorder(1);
    _f.setTopBorderBrush(borderBrush());
    _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  }
  _f.setPadding(2);
  return _f;
}

const QTextBlockFormat APrintingBody::blockFormat(Qt::Alignment align) const {
  QTextBlockFormat _f;
  _f.setAlignment(align);
  return _f;
}

const QTextBlockFormat APrintingBody::alignLeft() {
  QTextBlockFormat _f = blockFormat(Qt::AlignLeft | Qt::AlignVCenter);
  _f.setLeftMargin(p_margin);
  return _f;
}

const QTextBlockFormat APrintingBody::alignRight() {
  QTextBlockFormat _f = blockFormat(Qt::AlignRight | Qt::AlignVCenter);
  _f.setRightMargin(p_margin);
  return _f;
}

const QTextBlockFormat APrintingBody::alignCenter() {
  return blockFormat(Qt::AlignCenter | Qt::AlignVCenter);
}

void APrintingBody::insertText(const QString &text) {
  QTextBlockFormat _bf = blockFormat(Qt::AlignLeft | Qt::AlignTop);
  _bf.setProperty(QTextFormat::LayoutDirection, Qt::LeftToRight);
  _bf.setProperty(QTextFormat::BlockLeftMargin, p_margin);
  _bf.setProperty(QTextFormat::BlockRightMargin, p_margin);

  QTextCursor _cursor = textCursor();
  _cursor.insertText(carriageReturn());
  _cursor.setBlockFormat(_bf);
  _cursor.beginEditBlock();
  _cursor.insertText(text);
  _cursor.endEditBlock();
  _cursor.insertText(carriageReturn());
}

void APrintingBody::insertText(const QTextCursor &cursor,
                               const QString &text) const {
  if (cursor.isNull())
    return;

  QTextCursor _cursor(cursor);
  _cursor.beginEditBlock();
  _cursor.insertText(text);
  _cursor.endEditBlock();
  _cursor.insertText(carriageReturn());
}

void APrintingBody::setCellItem(QTextTableCell &cell, const QVariant &value,
                                Qt::Alignment align) {
  // qDebug() << Q_FUNC_INFO  << cell.row() << cell.column();
  QTextCursor cursor = cell.firstCursorPosition();
  cursor.setBlockFormat(blockFormat(align));
  cursor.beginEditBlock();
  switch (value.metaType().id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong): {
    qint64 _value = value.toLongLong();
    cursor.insertText(QString::number(_value));
  } break;

  case (QMetaType::Double): {
    double _value = value.toDouble();
    cursor.insertText(QString::number(_value));
  } break;

  default:
    cursor.insertText(value.toString());
    break;
  }
  cursor.endEditBlock();
}

} // namespace AntiquaCRM
