// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingformat.h"

#include <AntiquaCRM>

namespace AntiquaCRM {

APrintingFormat::APrintingFormat(AntiquaCRM::ASettings *config)
    : QObject{config}, cfg{config} {
  Q_CHECK_PTR(cfg);
  initConfiguration();
}

APrintingFormat::~APrintingFormat() {
  cfg->endGroup();
  cfg->deleteLater();
}

void APrintingFormat::initConfiguration() {
  cfg->beginGroup("printer");
  marginLeft = cfg->value("page_margin_left", 20.0).toReal();
  marginRight = cfg->value("page_margin_right", 20.0).toReal();
  marginSubject = cfg->value("page_margin_subject", 120).toReal();
  marginBody = cfg->value("page_margin_body", 15).toReal();
  marginRecipient = cfg->value("page_margin_recipient", 5.0).toReal();

  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query("SELECT ac_class,ac_value FROM antiquacrm_company;");
  if (q.size() > 0) {
    while (q.next()) {
      QString key = q.value("ac_class").toString().toUpper();
      QString value = q.value("ac_value").toString();
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << key << ":" << value;
#endif
      p_companyData.insert(key, value);
    }
  }
}

const QPageSize APrintingFormat::pageSize(QPageSize::PageSizeId id) const {
  QPageSize init(id);
  return QPageSize(init.definitionSize(), QPageSize::Point, init.name(),
                   QPageSize::ExactMatch);
}

const QPageLayout APrintingFormat::pageLayout(QPageSize::PageSizeId id) const {
  QPageLayout _layout;
  _layout.setOrientation(QPageLayout::Portrait);
  _layout.setPageSize(QPageSize(id));
  _layout.setMinimumMargins(QMargins(0, 0, 0, 0));
  _layout.setMargins(QMargins(marginLeft, 0, marginRight, 0));
  _layout.setMode(QPageLayout::FullPageMode);
  return _layout;
}

const QPen APrintingFormat::penStyle() const {
  QPen _pen(Qt::gray, Qt::SolidLine);
  _pen.setCapStyle(Qt::FlatCap);
  _pen.setWidthF(0.6);
  _pen.setCosmetic(true);
  return _pen;
}

const QBrush APrintingFormat::borderBrush() const {
  return QBrush(Qt::gray, Qt::SolidPattern);
}

const QTextCharFormat APrintingFormat::charFormat(const QFont &font,
                                                  bool bolded) {
  QTextCharFormat _format;
  QFont _font(font);
  _font.setBold(bolded);
  _format.setFont(_font);
  return _format;
}

const QTextBlockFormat APrintingFormat::alignRight() {
  QTextBlockFormat _format;
  _format.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return _format;
}

const QTextBlockFormat APrintingFormat::alignCenter() {
  QTextBlockFormat _format;
  _format.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return _format;
}

const QTextTableFormat APrintingFormat::tableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setTopMargin(0);
  _f.setBottomMargin(0);
  _f.setLeftMargin(marginLeft);
  _f.setRightMargin(marginRight);
  _f.setBorder(0.0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  _f.setAlignment(Qt::AlignRight | Qt::AlignTop);
  return _f;
}

const QTextTableFormat APrintingFormat::inlineTableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setMargin(0);
  _f.setBorder(0.0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  _f.setAlignment(Qt::AlignRight | Qt::AlignTop);
  return _f;
}

const QTextTableCellFormat APrintingFormat::recipientCellFormat() {
  QTextTableCellFormat _f;
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorder(0);
  _f.setPadding(0);
  _f.setTopPadding(marginRecipient);
  _f.setFont(getFont("print_font_address"));
  return _f;
}

const QTextTableCellFormat APrintingFormat::cellFormat(Qt::Alignment border) {
  QTextTableCellFormat _f;
  _f.setBorderBrush(borderBrush());
  _f.setPadding(0);
  if (border == Qt::AlignTop) {
    _f.setTopBorder(1);
    _f.setBottomBorder(0);
    _f.setBorderBrush(borderBrush());
    _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  } else if (border == Qt::AlignBottom) {
    _f.setTopBorder(0);
    _f.setBottomBorder(1);
    _f.setBorderBrush(borderBrush());
    _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  } else {
    _f.setTopBorder(0);
    _f.setBottomBorder(0);
    _f.setBorderBrush(QBrush(Qt::NoBrush));
    _f.setTopBorderStyle(QTextFrameFormat::BorderStyle_None);
  }
  return _f;
}

const QString APrintingFormat::companyData(const QString &key) {
  return p_companyData.value(key.toUpper());
}

const QStringList APrintingFormat::fontKeys() const {
  QStringList _keys;
  foreach (const QString _k, cfg->childKeys()) {
    if (_k.startsWith("print_font_", Qt::CaseSensitive))
      _keys.append(_k);
  }
  return _keys;
}

const QFont APrintingFormat::getFont(const QString &key) const {
  QFont font;
  if (!font.fromString(cfg->value(key).toString())) {
    qWarning("Can't load Printing '%s' font!", qPrintable(key));
  }
  return font;
}

} // namespace AntiquaCRM
