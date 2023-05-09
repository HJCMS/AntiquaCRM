// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintingpage.h"

#include <AntiquaCRM>

#ifdef ANTIQUA_DEVELOPEMENT
#define DEBUG_DISPLAY_BORDERS 1
#endif

namespace AntiquaCRM {

APrintingPage::APrintingPage(QWidget *parent) : QTextEdit{parent} {
  setAttribute(Qt::WA_OpaquePaintEvent, true);
  QPalette _p = palette();
  _p.setColor(QPalette::Base, Qt::white);
  _p.setColor(QPalette::Text, Qt::black);
  setPalette(_p);
  setTextColor(Qt::black);

  const QRectF _rf = pointsRect();
  setFixedSize(_rf.width(), _rf.height());

  cfg = new AntiquaCRM::ASettings(this);
  initConfiguration();

  QTextFrameFormat frameFormat;
  frameFormat.setLeftMargin((marginLeft * points));
  frameFormat.setRightMargin((marginRight * points));
  frameFormat.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  mainFrame = document()->rootFrame();
  mainFrame->setFrameFormat(frameFormat);
}

APrintingPage::~APrintingPage() {
  cfg->endGroup();
  cfg->deleteLater();
}

void APrintingPage::initConfiguration() {
  cfg->beginGroup("printer");
  marginLeft = cfg->value("page_margin_left", 20.0).toReal();
  marginRight = cfg->value("page_margin_right", 20.0).toReal();
  marginSubject = cfg->value("page_margin_subject", 120).toReal();
  marginBody = cfg->value("page_margin_body", 15).toReal();
  addressIndent = cfg->value("page_margin_recipient", 0.5).toReal();

  QString _sql_str("SELECT ac_class,ac_value FROM antiquacrm_company");
  _sql_str.append(" ORDER BY ac_class;");

  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query(_sql_str);
  if (q.size() > 0) {
    while (q.next()) {
      QString key = q.value("ac_class").toString().toUpper();
      QString value = q.value("ac_value").toString();
      // #ifdef ANTIQUA_DEVELOPEMENT
      //       qDebug() << Q_FUNC_INFO << key << ":" << value;
      // #endif
      p_companyData.insert(key, value);
    }
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else if (!sql.lastError().isEmpty()) {
    qDebug() << Q_FUNC_INFO << sql.lastError();
  }
#endif
}

void APrintingPage::paintEvent(QPaintEvent *event) {
  QPainter painter;
  QBrush background(Qt::white, Qt::SolidPattern);
  painter.begin(viewport());
  painter.fillRect(viewport()->rect(), background);
  // Letter folding lines
  int _yh = (rect().height() / 3);
  int _ym = (rect().height() / 2);
  painter.translate(0, 0);
  painter.setPen(QPen(Qt::gray));
  painter.drawLine(QPoint(5, _yh), // start
                   QPoint((borderLeft() / 2), _yh));
  painter.drawLine(QPoint(5, _ym), // start
                   QPoint(((borderLeft() / 3) * 2), _ym));
#ifdef DEBUG_DISPLAY_BORDERS
  // show borders
  painter.drawLine(QPoint(borderLeft(), 0),
                   QPoint(borderLeft(), rect().height()));
  painter.drawLine(QPoint(borderRight(), 0),
                   QPoint(borderRight(), rect().height()));
#endif

  painter.translate(borderLeft(), 0);
  const QImage image = watermark();
  if (!image.isNull()) {
    painter.setOpacity(watermarkOpacity());
    painter.drawImage(QPointF(5, 5), image);
    // letter window
    QRectF _wr = letterWindowRect();
    _wr.setX(_wr.x() - borderLeft());
    painter.setPen(QPen(Qt::gray));
    painter.setOpacity(0.8);
    painter.fillRect(_wr, background);
  }
  painter.end();
  QTextEdit::paintEvent(event);
}

void APrintingPage::setLetterHeading() {
  QString _title = companyData("COMPANY_PRINTING_HEADER");
  QTextBlockFormat _block;
  _block.setAlignment(Qt::AlignCenter);
  QFont _font = getFont("print_font_header");
  QTextCursor _cursor = mainFrame->firstCursorPosition();
  _cursor.setCharFormat(charFormat(_font));
  foreach (QString line, _title.split("#")) {
    _cursor.insertBlock(_block);
    _cursor.insertText(line);
    _cursor.atEnd();
  }
}

QTextTable *APrintingPage::recipientAddress(const QString &subject) {
  QTextCursor cursor = textCursor();
  QFont font(getFont("print_font_small"));
  font.setUnderline(true);
  font.setPointSize(8);

  QString company(companyData("COMPANY_SHORTNAME"));
  company.append(" - ");
  company.append(companyData("COMPANY_STREET"));
  company.append(" - ");
  company.append(companyData("COMPANY_LOCATION"));

  QTextTableFormat _tableFormat = tableFormat();
  _tableFormat.setLeftMargin((25 - marginLeft) * points);
  _tableFormat.setTopMargin(font.pointSize() * 2);

  QTextTable *table = cursor.insertTable(2, 2, _tableFormat);

  QTextCharFormat cellFormat;
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);

  QTextTableCell tc00 = table->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  cursor.insertText(company);

  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(subject);
  return table;
}

const QMap<QString, QVariant> APrintingPage::queryCustomerData(qint64 cId) {
  QMap<QString, QVariant> _map;
  AntiquaCRM::ASqlFiles sql_file("query_customer_recipient_address");
  if (!sql_file.openTemplate() || cId < 1) {
    qWarning("Can't open query_customer_recipient_address template!");
    return _map;
  }

  QString _clause("c_id='");
  _clause.append(QString::number(cId));
  _clause.append("'");
  sql_file.setWhereClause(_clause);

  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query(sql_file.getQueryContent());
  if (q.size() > 0) {
    _map.clear();
    while (q.next()) {
      _map.insert("id", q.value("cid").toLongLong());
      _map.insert("gender", q.value("gender").toInt());
      _map.insert("person", q.value("person").toString());
      _map.insert("address", q.value("address").toString());
    }
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else if (!sql.lastError().isEmpty()) {
    qDebug() << Q_FUNC_INFO << sql.lastError();
  }
#endif
  return _map;
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

const QTextTableFormat APrintingPage::tableFormat() {
  QTextTableFormat _f;
  _f.setWidth(mainFrame->frameFormat().width());
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
  _f.setAlignment(Qt::AlignLeft | Qt::AlignTop);
  return _f;
}

const QTextTableFormat APrintingPage::inlineTableFormat() {
  QTextTableFormat _f;
  _f.setWidth(QTextLength(QTextLength().PercentageLength, 100));
  _f.setPadding(0);
  _f.setCellPadding(0);
  _f.setCellSpacing(0);
  _f.setMargin(0);
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
  _f.setTopPadding(addressIndent);
  _f.setLeftPadding(addressIndent);
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

const QRectF APrintingPage::letterRect() const {
  QPageLayout _layout(QPageSize(QPageSize::A4), QPageLayout::Portrait,
                      QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter,
                      QMarginsF(0, 0, 0, 0));
  return _layout.fullRect();
}

const QRectF APrintingPage::pointsRect() const {
  QRectF _lr = letterRect();
  return QRectF(0, 0, (_lr.width() * points), (_lr.height() * points));
}

qreal APrintingPage::getPoints(int mm) const { return qRound(mm * points); }

const QRectF APrintingPage::letterWindowRect() const {
  QRectF _r(qRound(25 * points), // 25mm left
            qRound(45 * points), // 25mm top
            qRound(80 * points), // 80mm width
            qRound(45 * points)  // 45mm height
  );
  return _r;
}

qreal APrintingPage::borderLeft() const { return qRound(marginLeft * points); }

qreal APrintingPage::borderRight() const {
  qreal _w = (letterRect().width() * points);
  return qRound(_w - (marginRight * points));
}

qreal APrintingPage::inlineFrameWidth() const {
  qreal _w = (letterRect().width() * points);
  return qRound(_w - (marginLeft * points) - (marginRight * points));
}

} // namespace AntiquaCRM
