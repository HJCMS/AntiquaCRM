// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingpage.h"

#include <QFontMetrics>
#include <QPalette>
#include <QSizePolicy>
#include <QTableWidgetItem>

// #ifdef ANTIQUA_DEVELOPMENT
// Display helper borders
// #define PRINTPAGE_DEBUG
// #endif

namespace AntiquaCRM {

APrintingPage::APrintingPage(QWidget *parent, QPageSize::PageSizeId id)
    : QWidget{parent}, APrintTools{id} {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  QPalette _p = palette();
  _p.setColor(QPalette::Base, Qt::white);
  _p.setColor(QPalette::Text, Qt::black);
  setPalette(_p);

  cfg = new AntiquaCRM::ASettings(this);
  m_sql = new AntiquaCRM::ASqlCore(this);
  contentData = QJsonObject();
  normalFont = font();

  initConfiguration();

  const QRectF _rf = pagePoints();
  setFixedSize(_rf.width(), _rf.height());
}

APrintingPage::~APrintingPage() {
  if (!cfg->group().isEmpty())
    cfg->endGroup();

  foreach (QString _k, contentData.keys()) {
    contentData.remove(_k);
  }
  p_companyData.clear();
  cfg->deleteLater();
}

void APrintingPage::initConfiguration() {
  // NOTE: do not close the group in this function! @see destructor
  cfg->beginGroup("printer");

  QMarginsF _m(getPoints(cfg->value("page_margin_left", 20.0).toReal()),
               getPoints(cfg->value("page_margin_top", 0.0).toReal()),
               getPoints(cfg->value("page_margin_right", 20.0).toReal()),
               getPoints(cfg->value("page_margin_bottom", 0.0).toReal()));
  margin = _m;

  // Fonts
  headerFont = getFont("header");
  normalFont = getFont("normal");
  addressFont = getFont("address");
  footerFont = getFont("footer");

  watermark_opacity = cfg->value("print_watermark_opacity", 0.6).toReal();

  AntiquaCRM::ASqlFiles _tpl("query_company_data");
  if (!_tpl.openTemplate()) {
    qFatal("Can not read printing config!");
    return;
  }

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  if (_query.size() > 0) {
    while (_query.next()) {
      QString _key = _query.value("ac_class").toString().toUpper();
      QString _value = _query.value("ac_value").toString();
      if (_value.length() > 2)
        p_companyData.insert(_key, _value.trimmed());
    }
  } else {
    qWarning("No Printer Company data found!");
  }

  QString _company(companyData("COMPANY_SHORTNAME"));
  _company.append(" - ");
  _company.append(companyData("COMPANY_STREET"));
  _company.append(" - ");
  _company.append(companyData("COMPANY_LOCATION"));
  p_companyData.insert("COMPANY_ADDRESS_LABEL", _company);

#ifdef PRINTPAGE_DEBUG
  QHashIterator<QString, QString> it(p_companyData);
  while (it.hasNext()) {
    it.next();
    if (it.value().length() > 50) {
      qDebug() << it.key() << it.value().left(50) << "..";
    } else {
      qDebug() << it.key() << it.value();
    }
  }
#endif
}

void APrintingPage::paintHeader(QPainter &painter) {
  // DIN 5008 Letter Header
  const QRectF _rect(QPointF(borderLeft(), 0),
                     QPointF(borderRight(), getPoints(45)));
  painter.setFont(headerFont);
  painter.setPen(fontPen());

  const QImage _image = watermark();
  if (!_image.isNull()) {
    painter.setOpacity(watermark_opacity);
    painter.drawImage(_rect.topLeft(), _image);
    painter.setOpacity(1.0);
  }

  const QString _text = companyData("COMPANY_PRINTING_HEADER");
  painter.drawText(_rect, (Qt::AlignCenter | Qt::AlignTop), _text);
}

void APrintingPage::paintAddressBox(QPainter &painter) {
  // DIN 5008B Address Window position
  const QRectF _lwr = letterWindow();
  painter.setFont(footerFont);
  painter.setOpacity(0.8);
  painter.setPen(linePen());
  painter.fillRect(_lwr, QBrush(Qt::white, Qt::SolidPattern));

  // Company Address Header
  const QRectF _rect(QPointF(_lwr.left(), _lwr.top() - getPoints(5)),
                     QPointF(borderRight(), _lwr.bottom()));
  painter.setPen(fontPen());
  painter.drawText(_rect, companyData("COMPANY_ADDRESS_LABEL"));
  painter.setOpacity(1.0); // reset

  // Recipient Address Body
  const QPointF _top = addressZone().topLeft();
  const QString _address = toRichText(contentData.value("address").toString());

  QStaticText _recipient = textBlock((Qt::AlignLeft | Qt::AlignTop),
                                     QTextOption::WrapAtWordBoundaryOrAnywhere);
  _recipient.setTextWidth(getPoints(80));
  _recipient.setText(_address);
  _recipient.prepare(painter.transform(), addressFont);

  painter.setFont(addressFont);
  painter.drawStaticText(_top, _recipient);
}

void APrintingPage::paintIdentities(QPainter &painter) {
  const QFont _font = getFont("normal");
  // DIN 5008B Address Area
  const QPointF _point(getPoints(125), getPoints(55));
  painter.setPen(fontPen());
  painter.setFont(_font);
  painter.setOpacity(1.0);

  QStringList _txt;
  QStringList _keys({"order_id", "invoice_id", "customer_id", "delivery_id"});
  foreach (QString _k, _keys) {
    if (contentData.contains(_k)) {
      QJsonArray _arr = contentData.value(_k).toArray();
      _txt << _arr[0].toString() + " : " + _arr[1].toString();
    }
  }

  QStaticText _info;
  _info.setTextFormat(Qt::RichText);
  _info.setTextOption(textOption(Qt::AlignRight));
  _info.setText(_txt.join("<br>"));
  _info.prepare(painter.transform(), _font);
  painter.drawStaticText(_point, _info);
}

void APrintingPage::paintFooter(QPainter &painter) {
  // Footer line
  painter.setPen(linePen());
  painter.drawLine(footerLine());
  // Footer text
  painter.setPen(fontPen());
  painter.setFont(footerFont);

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

  QStaticText _leftBox = textBlock((Qt::AlignLeft | Qt::AlignTop));
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

  QStaticText _rightBox = textBlock((Qt::AlignLeft | Qt::AlignTop));
  _rightBox.setText(_body.join("<br>"));
  // END:Right

  // start positioning
  int _spacing = 6;
  int _height = -1;
  if (_leftBox.size().height() > _rightBox.size().height()) {
    _height = _leftBox.size().height();
  } else {
    _height = _rightBox.size().height();
  }
  // footer text boxes y Position.
  int _ft_y = (height() - _height);

  // float left
  // @note must painted before calculate the right box!
  int _left_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(borderLeft(), _left_box_y), _leftBox);

  // footer text box right x()
  int _lb_right = (borderLeft() + _leftBox.size().width());
  int _right_box_x = (width() / 2);
  if (_lb_right > _right_box_x)
    _right_box_x = (_lb_right + _spacing);

  // float right
  int _right_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(_right_box_x, _right_box_y), _rightBox);
}

void APrintingPage::paintEvent(QPaintEvent *event) {
  // Config Defaults
  const QBrush _base(Qt::white, Qt::SolidPattern);
  // Start Painter
  QPainter painter;
  painter.begin(this);
  painter.fillRect(rect(), _base);
  painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
  painter.translate(0, 0);

  // only when A4 letter is set
  if (pageLayout().pageSize().id() == QPageSize::A4) {
    // BEGIN::Letter_folding_lines
    int _yh = getPoints(105);
    int _ym = getPoints(148);
    painter.setPen(linePen());
    painter.drawLine(QPoint(5, _yh), // start
                     QPoint((borderLeft() / 2), _yh));
    painter.drawLine(QPoint(5, _ym), // start
                     QPoint(((borderLeft() / 3) * 2), _ym));
    // END::Letter_folding_lines
#ifdef PRINTPAGE_DEBUG
    const QRectF _frame(
        QPointF(borderLeft(), 0),                     // top left
        QPointF(borderRight(), pagePoints().height()) // bottom right
    );
    painter.setPen(QPen(Qt::yellow));
    painter.drawLine(_frame.topLeft(), _frame.bottomLeft());
    painter.drawLine(_frame.topRight(), _frame.bottomRight());
#endif

    // BEGIN::Header
    paintHeader(painter);
    // END::Header

    // BEGIN::Address
    paintAddressBox(painter);
    // END::Address

    // BEGIN::Identities
    paintIdentities(painter);
    // END::Identities

    // BEGIN::Body
    paintContent(painter);
    // END::Body

    // BEGIN::Footer
    paintFooter(painter);
    // END::Footer
  }
  painter.end();
  QWidget::paintEvent(event);
}

const QFont APrintingPage::getFont(const QString &key) const {
  QFont _font;
  QString _str("print_font_");
  _str.append(key);
  if (!_font.fromString(cfg->value(_str).toString())) {
    qWarning("Can not load Printing '%s' font!", qPrintable(_str));
  }
  return _font;
}

const QLineF APrintingPage::footerLine() const {
  qreal _y = qRound(pagePoints().height() - margin.bottom());
  const QPointF _sp(margin.left(), _y);
  return QLineF(_sp, QPointF(borderRight(), _sp.y()));
}

qreal APrintingPage::borderLeft() const { return margin.left(); }

qreal APrintingPage::borderRight() const {
  return qRound(pagePoints().width() - margin.right());
}

qreal APrintingPage::inlineWidth() const {
  return qRound(borderRight() - borderLeft());
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
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << _sql;
#else
    qWarning("Printer Recipient Data is empty!");
#endif
  }
  return _map;
}

const QString APrintingPage::companyData(const QString &key) {
  return p_companyData.value(key.toUpper());
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
#ifdef ANTIQUA_DEVELOPMENT
  else {
    qDebug() << Q_FUNC_INFO << cfg->group()
             << cfg->value("print_attachments_path").toString()
             << cfg->value("print_watermark_file").toString();
  }
#endif
  return QImage();
}

} // namespace AntiquaCRM
