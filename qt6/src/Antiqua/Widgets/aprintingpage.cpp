// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingpage.h"

#include <QFontMetrics>
#include <QPalette>
#include <QSizePolicy>
#include <QTableWidgetItem>

#ifdef ANTIQUA_DEVELOPEMENT
// Display helper borders
#define PRINTPAGE_DISPLAY_BORDERS 1
#endif

namespace AntiquaCRM {

APrintingPage::APrintingPage(QWidget *parent, QPageSize::PageSizeId id)
    : QWidget{parent}, APrintTools{id} {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  QPalette _p = palette();
  _p.setColor(QPalette::Base, Qt::white);
  _p.setColor(QPalette::Text, Qt::black);
  setPalette(_p);

  contentData = QJsonObject();
  cfg = new AntiquaCRM::ASettings(this);
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
  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  // Payment Settings
  cfg->beginGroup("payment");
  foreach (QString _k, cfg->allKeys()) {
    p_companyData.insert(_k.toUpper(), cfg->value(_k).toString());
  }
  cfg->endGroup();

  // NOTE: do not close this group here!
  cfg->beginGroup("printer");
  margin.left = cfg->value("page_margin_left", 20.0).toReal();
  margin.right = cfg->value("page_margin_right", 20.0).toReal();
  margin.subject = cfg->value("page_margin_subject", 120).toReal();
  margin.top = cfg->value("page_margin_body", 20).toReal();
  margin.address = cfg->value("page_margin_recipient", 0.5).toReal();

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
  p_companyData.insert("COMPANY_ADDRESS_LABEL", _company);
  p_companyData.insert("COMPANY_INTRO_DELIVERY",
                       tr("Thank you for your order."));
}

void APrintingPage::paintHeader(QPainter &painter) {
  painter.setPen(fontPen());
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

  int _x = qRound((width() / 2) - (_box.size().width() / 2));

  painter.setFont(getFont("print_font_header"));
  painter.drawStaticText(QPoint(_x, margin.top), _box);
}

void APrintingPage::paintAddressBox(QPainter &painter) {
  const QFont _small_font(getFont("print_font_small"));
  const QFont _address_font(getFont("print_font_address"));
  // letter address window
  const QRectF _wr = letterWindow();
  painter.setPen(linePen());
  painter.setOpacity(0.8);
  painter.fillRect(_wr, QBrush(Qt::white, Qt::SolidPattern));
  painter.setOpacity(1.0);

  QTextOption _opts(Qt::AlignLeft);
  _opts.setWrapMode(QTextOption::NoWrap);

  QString _txt = companyData("COMPANY_ADDRESS_LABEL");
  QStaticText _company;
  _company.setTextFormat(Qt::PlainText);
  _company.setTextOption(_opts);
  _company.setText(_txt.trimmed());
  _company.prepare(painter.transform(), _small_font);
  QRect _txt_rect = QFontMetrics(_small_font).boundingRect(_company.text());
  int _y = (_wr.top() - _txt_rect.height());
  painter.setOpacity(0.8);
  painter.setPen(fontPen());
  painter.setFont(_small_font);
  painter.drawStaticText(QPoint(_wr.left(), _y), _company);
  painter.setOpacity(1.0);
  // Address Body
  const QPoint _rp(_wr.left() + getPoints(5), _wr.top() + getPoints(5));
  _txt = toRichText(contentData.value("address").toString());
  _txt_rect = QFontMetrics(_address_font).boundingRect(_txt);
  _opts.setWrapMode(QTextOption::ManualWrap);

  QStaticText _address;
  _address.setTextFormat(Qt::RichText);
  _address.setTextOption(_opts);
  _address.setText(_txt.trimmed());
  _address.prepare(painter.transform(), _address_font);

  painter.setPen(fontPen());
  painter.setFont(_address_font);
  painter.drawStaticText(_rp, _address);
}

void APrintingPage::paintIdentities(QPainter &painter) {
  const QFont _font = getFont("print_font_normal");
  const QPoint _point(getPoints(125), getPoints(50));
  painter.setPen(fontPen());
  painter.setFont(_font);

  QStringList _txt;
  QStringList _keys({"order_id", "invoice_id", "customer_id", "delivery_id"});
  foreach (QString _k, _keys) {
    if (contentData.contains(_k)) {
      QJsonArray _arr = contentData.value(_k).toArray();
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

void APrintingPage::paintFooter(QPainter &painter) {
  painter.setPen(fontPen());
  painter.setFont(getFont("print_font_footer"));

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
  int _ft_y = (height() - _height);

  // float left
  // @note must painted before calculate the right box!
  int _left_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(_left_x, _left_box_y), _leftBox);

  // footer text box right x()
  int _lb_right = (borderLeft() + _leftBox.size().width());
  int _right_box_x = (width() / 2);
  if (_lb_right > _right_box_x)
    _right_box_x = (_lb_right + _spacing);

  // float right
  int _right_box_y = (_ft_y - _spacing);
  painter.drawStaticText(QPoint(_right_box_x, _right_box_y), _rightBox);

  // prepend heading line
  int _line_y = (_ft_y - 10);
  painter.setPen(linePen());
  painter.drawLine(QPoint(_left_x, _line_y), QPoint(_right_x, _line_y));
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
#ifdef PRINTPAGE_DISPLAY_BORDERS
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

void APrintingPage::setArticleHeaderItem(int column, const QString &title,
                                         Qt::Alignment align) {
  if (m_table == nullptr)
    return;

  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setForeground(Qt::black);
  item->setBackground(Qt::white);
  item->setTextAlignment(align);
  m_table->setHorizontalHeaderItem(column, item);
}

qreal APrintingPage::borderLeft() const { return getPoints(margin.left); }

qreal APrintingPage::borderRight() const {
  return qRound(pagePoints().width() - getPoints(margin.right));
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
#ifdef ANTIQUA_DEVELOPEMENT
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

} // namespace AntiquaCRM
