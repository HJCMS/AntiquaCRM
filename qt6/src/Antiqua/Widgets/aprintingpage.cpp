// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingpage.h"

#include <QFontMetrics>
#include <QPrinter>
#include <QPrinterInfo>
#include <QRegularExpression>
#include <QTextLayout>
#include <QTextLine>

#ifdef ANTIQUA_DEVELOPEMENT
#define DEBUG_DISPLAY_BORDERS 1
#endif

namespace AntiquaCRM {

APrintingPage::APrintingPage(QWidget *parent, QPageSize::PageSizeId id)
    : QTextEdit{parent}, p_pageSizeId{id} {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  QPalette _p = palette();
  _p.setColor(QPalette::Base, Qt::white);
  _p.setColor(QPalette::Text, Qt::black);
  setPalette(_p);
  setTextColor(Qt::black);

  const QRectF _rf = pointsRect();
  setFixedSize(_rf.width(), _rf.height());
  setMaximumSize(pointsRect().size().toSize());

  p_content_data = QJsonObject();
  cfg = new AntiquaCRM::ASettings(this);
  initConfiguration();

  QTextFrameFormat frameFormat;
  if (pageLayout().pageSize().id() == QPageSize::A4) {
    frameFormat.setLeftMargin(margin.left * points);
    frameFormat.setRightMargin(margin.right * points);
  } else {
    frameFormat.setLeftMargin(5 * points);
    frameFormat.setRightMargin(5 * points);
  }
  frameFormat.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  frameFormat.setHeight(QTextLength(QTextLength().PercentageLength, 100));
  rootFrame = document()->rootFrame();
  rootFrame->setFrameFormat(frameFormat);
}

APrintingPage::~APrintingPage() {
  if (!cfg->group().isEmpty())
    cfg->endGroup();

  p_companyData.clear();
  cfg->deleteLater();
}

const QString APrintingPage::toRichText(const QString &txt) const {
  static const QRegularExpression pattern("[\\n\\r]",
                                          QRegularExpression::NoPatternOption);
  QString _txt(txt.trimmed());
  _txt.replace(pattern, "<br>");
  return _txt;
}

void APrintingPage::initConfiguration() {
  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  // NOTE: do not close this group here!
  cfg->beginGroup("printer");
  margin.left = cfg->value("page_margin_left", 20.0).toReal();
  margin.right = cfg->value("page_margin_right", 20.0).toReal();
  margin.subject = cfg->value("page_margin_subject", 120).toReal();
  margin.top = cfg->value("page_margin_body", 20).toReal();
  margin.address = cfg->value("page_margin_recipient", 0.5).toReal();

  QStringList printers = QPrinterInfo::availablePrinterNames();
  if (printers.size() > 0) {
    QString a4 = cfg->value("device_primary").toString();
    p_printerInfo.dinA4 = QPrinterInfo::printerInfo(a4);
    QString a6 = cfg->value("device_secondary").toString();
    p_printerInfo.dinA6 = QPrinterInfo::printerInfo(a6);
  } else {
    qWarning("No Printer configuration found!");
  }

  QString _sql("SELECT ac_class, ac_value FROM antiquacrm_company");
  _sql.append(" ORDER BY ac_class;");

  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() > 0) {
    while (_query.next()) {
      QString _key = _query.value("ac_class").toString().toUpper();
      QString _value = _query.value("ac_value").toString();
      p_companyData.insert(_key, _value);
    }
  } else {
    qWarning("No Printer Company data found!");
  }

  QString _company(companyData("COMPANY_SHORTNAME"));
  _company.append(" - ");
  _company.append(companyData("COMPANY_STREET"));
  _company.append(" - ");
  _company.append(companyData("COMPANY_LOCATION"));
  p_companyData.insert("COMPANY_LETTER_STRING", _company);
}

void APrintingPage::paintHeader(QPainter &painter) {
  painter.setPen(fontPen);
  const QImage image = watermark();
  if (!image.isNull()) {
    painter.setOpacity(watermarkOpacity());
    painter.drawImage(QPointF(borderLeft(), 0), image);
    painter.setOpacity(1.0);
  }

  QString _txt = toRichText(companyData("COMPANY_PRINTING_HEADER"));
  QTextOption _opts(Qt::AlignCenter);
  _opts.setWrapMode(QTextOption::ManualWrap);

  QStaticText _box;
  _box.setTextFormat(Qt::RichText);
  _box.setTextOption(_opts);
  _box.setText(_txt);
  _box.prepare(painter.transform(), getFont("print_font_header"));

  int _x = qRound((viewport()->width() / 2) - (_box.size().width() / 2));

  painter.setFont(getFont("print_font_header"));
  painter.drawStaticText(QPoint(_x, margin.top), _box);
}

void APrintingPage::paintAddressBox(QPainter &painter) {
  const QFont _small_font(getFont("print_font_small"));
  const QFont _address_font(getFont("print_font_address"));
  // letter address window
  const QRectF _wr = letterWindow();
  painter.setPen(linePen);
  painter.setOpacity(0.8);
  painter.fillRect(_wr, QBrush(Qt::white, Qt::SolidPattern));
  painter.setOpacity(1.0);

  QTextOption _opts(Qt::AlignLeft);
  _opts.setWrapMode(QTextOption::NoWrap);

  QString _txt = companyData("COMPANY_LETTER_STRING");
  QStaticText _company;
  _company.setTextFormat(Qt::PlainText);
  _company.setTextOption(_opts);
  _company.setText(_txt.trimmed());
  _company.prepare(painter.transform(), _small_font);
  QRect _txt_rect = QFontMetrics(_small_font).boundingRect(_company.text());
  int _y = (_wr.top() - _txt_rect.height());
  painter.setOpacity(0.8);
  painter.setPen(fontPen);
  painter.setFont(_small_font);
  painter.drawStaticText(QPoint(_wr.left(), _y), _company);
  painter.setOpacity(1.0);
  // Address Body
  const QPoint _rp(_wr.left() + getPoints(5), _wr.top() + getPoints(5));
  _txt = toRichText(p_content_data.value("address").toString());
  _txt_rect = QFontMetrics(_address_font).boundingRect(_txt);
  _opts.setWrapMode(QTextOption::WrapAnywhere);

  QStaticText _address;
  _address.setTextFormat(Qt::RichText);
  _address.setTextOption(_opts);
  _address.setText(_txt.trimmed());
  _address.prepare(painter.transform(), _address_font);

  painter.setPen(fontPen);
  painter.setFont(_address_font);
  painter.drawStaticText(_rp, _address);
}

void APrintingPage::paintIdentities(QPainter &painter) {
  const QFont _font = getFont("print_font_normal");
  const QPoint _point(getPoints(125), getPoints(50));
  painter.setPen(fontPen);
  painter.setFont(_font);

  QStringList _txt;
  QStringList _keys({"order_id", "invoice_id", "customer_id", "delivery_id"});
  foreach (QString _k, _keys) {
    if (p_content_data.contains(_k)) {
      QJsonArray _arr = p_content_data.value(_k).toArray();
      _txt << _arr[0].toString() + " : " + _arr[1].toString();
    }
  }

  QTextOption _opts(Qt::AlignRight);
  _opts.setWrapMode(QTextOption::NoWrap);

  QStaticText _info;
  _info.setTextFormat(Qt::RichText);
  _info.setTextOption(_opts);
  _info.setText(_txt.join("<br>"));
  _info.prepare(painter.transform(), _font);
  painter.drawStaticText(_point, _info);
}

void APrintingPage::paintSubject(QPainter &painter) {
  const QFont _font = getFont("print_font_normal");
  painter.setPen(fontPen);
  painter.setFont(_font);

  QTextOption _opts(Qt::AlignLeft);
  _opts.setWrapMode(QTextOption::NoWrap);

  QStaticText _info;
  _info.setTextFormat(Qt::PlainText);
  _info.setTextOption(_opts);
  _info.setText(p_content_data.value("subject").toString());
  _info.prepare(painter.transform(), _font);
  painter.drawStaticText(QPoint(borderLeft(), getPoints(98)), _info);

  _opts.setAlignment(Qt::AlignRight);
  QString _date_str(companyData("COMPANY_LOCATION_NAME"));
  _date_str.append(" " + tr("on") + " ");
  _date_str.append(QDate::currentDate().toString("dd.MM.yyyy"));

  QStaticText _date;
  _date.setTextFormat(Qt::PlainText);
  _date.setTextOption(_opts);
  _date.setText(_date_str);
  _date.prepare(painter.transform(), _font);
  int _x = (borderRight() - _date.size().width());
  painter.drawStaticText(QPoint(_x, getPoints(98)), _date);
}

void APrintingPage::paintFooter(QPainter &painter) {
  painter.setPen(fontPen);
  painter.setFont(getFont("print_font_footer"));

  QTextOption _textOpts(Qt::AlignLeft | Qt::AlignTop);
  _textOpts.setWrapMode(QTextOption::NoWrap);

  // BEGIN:Left
  QStringList _body;
  QString _buffer = companyData("COMPANY_FULLNAME");
  _body << _buffer.trimmed();
  _buffer = companyData("COMPANY_STREET");
  _buffer.append(" " + companyData("COMPANY_LOCATION"));
  _body << _buffer;
  _buffer = tr("eMail") + ": " + companyData("COMPANY_EMAIL");
  _body << _buffer;
  _buffer = tr("Phone") + ": " + companyData("COMPANY_PHONE");
  _body << _buffer;
  if (!companyData("COMPANY_FAX").isEmpty()) {
    _buffer = tr("Fax") + ": " + companyData("COMPANY_FAX");
    _body << _buffer;
  }

  QStaticText _leftBox;
  _leftBox.setTextFormat(Qt::RichText);
  _leftBox.setTextOption(_textOpts);
  _leftBox.setText(_body.join("<br>"));
  // END:Left

  // BEGIN:Right
  _body.clear();
  _buffer = tr("Bank") + ": " + companyData("COMPANY_BANK_NAME");
  _body << _buffer;
  _buffer = tr("Swift-BIC") + ": " + companyData("COMPANY_BANK_BICSWIFT");
  _body << _buffer;
  _buffer = tr("IBAN") + ": " + companyData("COMPANY_BANK_IBAN");
  _body << _buffer;
  _buffer = tr("VAT No.") + ": " + companyData("COMPANY_EPR_NUMBER");
  _body << _buffer;
  if (!companyData("COMPANY_PAYPAL_ID").isEmpty()) {
    _buffer = tr("PayPal") + ": " + companyData("COMPANY_PAYPAL_ID");
    _body << _buffer;
  }

  QStaticText _rightBox;
  _rightBox.setTextFormat(Qt::RichText);
  _rightBox.setTextOption(_textOpts);
  _rightBox.setText(_body.join("<br>"));
  // END:Right

  // start positioning
  qreal _left_x = borderLeft();
  qreal _right_x = borderRight();
  int _spacing = 6;
  int _height = -1;
  if (_leftBox.size().height() > _rightBox.size().height()) {
    _height = _leftBox.size().height();
  } else {
    _height = _rightBox.size().height();
  }
  // footer text boxes y Position.
  int _ft_y = (viewport()->height() - _height);

  // float left
  // @note must painted before calculate the right box!
  int _left_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(_left_x, _left_box_y), _leftBox);

  // footer text box right x()
  int _lb_right = (borderLeft() + _leftBox.size().width());
  int _right_box_x = (viewport()->width() / 2);
  if (_lb_right > _right_box_x)
    _right_box_x = (_lb_right + _spacing);

  // float right
  int _right_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(_right_box_x, _right_box_y), _rightBox);

  // prepend heading line
  int _line_y = (_ft_y - _spacing);
  painter.setPen(linePen);
  painter.drawLine(QPoint(_left_x, _line_y), QPoint(_right_x, _line_y));
}

void APrintingPage::paintEvent(QPaintEvent *event) {
  // Config Defaults
  const QBrush _base(Qt::white, Qt::SolidPattern);
  // Start Painter
  QPainter painter;
  painter.begin(viewport());
  painter.fillRect(viewport()->rect(), _base);
  painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
  painter.translate(0, 0);

  // only when A4 letter is set
  if (pageLayout().pageSize().id() == QPageSize::A4) {
    // BEGIN::Letter_folding_lines
    int _yh = getPoints(105);
    int _ym = getPoints(148);
    painter.setPen(linePen);
    painter.drawLine(QPoint(5, _yh), // start
                     QPoint((borderLeft() / 2), _yh));
    painter.drawLine(QPoint(5, _ym), // start
                     QPoint(((borderLeft() / 3) * 2), _ym));
#ifdef DEBUG_DISPLAY_BORDERS
    // Seitenränder Anzeigen!
    const QRectF _frame = paintArea();
    painter.setPen(QPen(Qt::yellow));
    painter.drawLine(_frame.topLeft(), _frame.bottomLeft());
    painter.drawLine(_frame.topRight(), _frame.bottomRight());
#endif
    // END::Letter_folding_lines

    // BEGIN::Header
    paintHeader(painter);
    // END::Header

    // BEGIN::Address
    paintAddressBox(painter);
    // END::Address

    // BEGIN::Identities
    paintIdentities(painter);
    // END::Identities

    // BEGIN::Subject
    paintSubject(painter);
    // END::Subject

    // BEGIN::Footer
    paintFooter(painter);
    // END::Footer
  }
  painter.end();
  QTextEdit::paintEvent(event);
}

qreal APrintingPage::getPoints(int millimeter) const {
  return qRound(millimeter * points);
}

const QRectF APrintingPage::pointsRect() const {
  const QRectF _lr = pageLayout().fullRect(QPageLayout::Millimeter);
  return QRectF(0, 0, (_lr.width() * points), (_lr.height() * points));
}

const QRectF APrintingPage::paintArea() const {
  return QRectF(QPointF(borderLeft(), 0),
                QPointF(borderRight(), pointsRect().height()));
}

const QRectF APrintingPage::letterWindow(qreal left) const {
  return QRectF(getPoints(left), // 25mm left
                getPoints(45),   // 45mm top
                getPoints(80),   // 80mm width
                getPoints(45)    // 45mm height
  );
}

qreal APrintingPage::borderLeft() const {
  // linker rand
  return qRound(margin.left * points);
}

qreal APrintingPage::borderRight() const {
  // rechter rand
  return qRound(pointsRect().width() - (margin.right * points));
}

const QPageLayout APrintingPage::pageLayout() const {
  QPageLayout _layout;
  _layout.setOrientation(QPageLayout::Portrait);
  _layout.setPageSize(QPageSize(p_pageSizeId));
  _layout.setMinimumMargins(QMargins(0, 0, 0, 0));
  _layout.setMargins(QMargins(0, 0, 0, 0));
  _layout.setUnits(QPageLayout::Millimeter);
  _layout.setMode(QPageLayout::FullPageMode);
  return _layout;
}

const QMap<QString, QVariant> APrintingPage::queryCustomerData(qint64 cid) {
  QMap<QString, QVariant> _map;
  AntiquaCRM::ASqlFiles _tpl("query_customer_recipient_address");
  if (!_tpl.openTemplate() || cid < 1) {
    qWarning("Unable to query customer recipient address template!");
    return _map;
  }

  QString _clause("c_id=");
  _clause.append(QString::number(cid));
  _tpl.setWhereClause(_clause);

  const QString _sql = _tpl.getQueryContent();
  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() > 0) {
    _map.clear();
    while (_query.next()) {
      _map.insert("id", cid);
      _map.insert("gender", _query.value("gender").toInt());
      _map.insert("person", _query.value("person").toString());
      _map.insert("address", _query.value("address").toString());
    }
    _query.clear();
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _sql;
#else
    qWarning("Printer Recipient Data is empty!");
#endif
  }
  return _map;
}

const QPrinterInfo APrintingPage::getPrinterInfo(QPageSize::PageSizeId id) {
  if (id == QPageSize::A6)
    return p_printerInfo.dinA6;

  return p_printerInfo.dinA4;
}

const QPen APrintingPage::penStyle() const {
  QPen _pen(Qt::gray, Qt::SolidLine);
  _pen.setCapStyle(Qt::FlatCap);
  _pen.setWidthF(0.6);
  _pen.setCosmetic(true);
  return _pen;
}

const QBrush APrintingPage::borderBrush() const {
  return QBrush(Qt::gray, Qt::SolidPattern);
}

const QTextCharFormat APrintingPage::charFormat(const QFont &font,
                                                bool bolded) {
  QTextCharFormat _format;
  QFont _font(font);
  _font.setBold(bolded);
  _format.setFont(_font);
  return _format;
}

const QTextBlockFormat APrintingPage::bodyText() {
  qreal _i = 5.0;
  QTextBlockFormat _f;
  _f.setAlignment(Qt::AlignLeft);
  _f.setLeftMargin(_i);
  _f.setTopMargin(_i);
  _f.setRightMargin(_i);
  _f.setBottomMargin(_i);
  return _f;
}

const QTextBlockFormat APrintingPage::alignRight() {
  QTextBlockFormat _format;
  _format.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return _format;
}

const QTextBlockFormat APrintingPage::alignCenter() {
  QTextBlockFormat _format;
  _format.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return _format;
}

const QTextCharFormat APrintingPage::verticalCenter() {
  QTextCharFormat _f;
  _f.setFont(getFont("print_font_normal"));
  _f.setVerticalAlignment(QTextCharFormat::AlignMiddle);
  return _f;
}

const QTextTableFormat APrintingPage::tableFormat() {
  QTextTableFormat _f;
  _f.setWidth(rootFrame->frameFormat().width());
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setTopMargin(0);
  _f.setBottomMargin(0);
  _f.setLeftMargin(0);
  _f.setRightMargin(0);
#ifdef DEBUG_DISPLAY_BORDERS
  _f.setBorder(1);
  _f.setBorderBrush(QBrush(Qt::gray));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
#else
  _f.setBorder(0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
#endif
  _f.setAlignment(Qt::AlignLeft);
  return _f;
}

const QTextTableFormat APrintingPage::inlineTableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 99.8));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setMargin(0);
  _f.setLeftMargin(2);
#ifdef DEBUG_DISPLAY_BORDERS
  _f.setBorder(1);
  _f.setBorderBrush(QBrush(Qt::gray));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
#else
  _f.setBorder(0);
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorderStyle(QTextFrameFormat::BorderStyle_None);
#endif
  _f.setAlignment(Qt::AlignLeft | Qt::AlignTop);
  return _f;
}

const QTextTableCellFormat APrintingPage::addressCellFormat() {
  QTextTableCellFormat _f;
  _f.setBorderBrush(QBrush(Qt::NoBrush));
  _f.setBorder(0);
  _f.setPadding(0);
  _f.setTopPadding(margin.address);
  _f.setLeftPadding(margin.address);
  _f.setFont(getFont("print_font_address"));
  return _f;
}

const QTextTableCellFormat APrintingPage::cellFormat(Qt::Alignment border) {
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

const QString APrintingPage::companyData(const QString &key) {
  return p_companyData.value(key.toUpper());
}

const QStringList APrintingPage::fontKeys() const {
  QStringList _keys;
  foreach (const QString _k, cfg->childKeys()) {
    if (_k.startsWith("print_font_", Qt::CaseSensitive))
      _keys.append(_k);
  }
  return _keys;
}

const QFont APrintingPage::getFont(const QString &key) const {
  QFont font;
  if (!font.fromString(cfg->value(key).toString())) {
    qWarning("Can't load Printing '%s' font!", qPrintable(key));
  }
  return font;
}

qreal APrintingPage::watermarkOpacity() const {
  return cfg->value("print_watermark_opacity", 0.6).toReal();
}

const QImage APrintingPage::watermark() const {
  QStringList accept({"PNG", "JPG"});
  QString file(cfg->value("print_attachments_path").toString());
  file.append(QDir::separator());
  file.append(cfg->value("print_watermark_file").toString());
  QFileInfo info(file);
  if (info.isFile() && info.isReadable()) {
    QString type = info.completeSuffix().split(".").last().toUpper();
    if (!accept.contains(type, Qt::CaseSensitive)) {
      qWarning("unsupported image type");
      return QImage();
    }
    QByteArray buffer;
    QFile fp(info.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      while (!fp.atEnd()) {
        buffer += fp.readLine();
      }
    }
    fp.close();
    return QImage::fromData(buffer, type.toLocal8Bit());
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << cfg->group()
             << cfg->value("print_attachments_path").toString()
             << cfg->value("print_watermark_file").toString();
  }
#endif
  return QImage();
}

bool APrintingPage::setContentData(const QJsonObject &data) {
  if (data.isEmpty())
    return false;

  p_content_data = data;
  return true;
}

} // namespace AntiquaCRM
