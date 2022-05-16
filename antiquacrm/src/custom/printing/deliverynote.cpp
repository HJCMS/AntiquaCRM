// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverynote.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "texteditor.h"

#include <QAbstractTextDocumentLayout>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QUrl>

static const QByteArray defaultCodec() {
  QTextCodec *c = QTextCodec::codecForLocale();
  return c->name();
}

DeliveryNote::DeliveryNote(QWidget *parent) : Printing{parent} {
  setObjectName("printing_delivery_note");
  setWindowTitle(tr("Delivery note"));

  config = new ApplSettings(this);

  printButton->setIcon(myIcon("printer"));
  connect(printButton, SIGNAL(clicked()), this, SLOT(printDeliveryNote()));
  readConfiguration();
}

void DeliveryNote::readConfiguration() {
  config->beginGroup("company");
  QStringList keys = config->childKeys();
  if (keys.count() < 1) {
    warningMessageBox(tr("<p>Your Company configuration is "
                         "incomplite!</p>Edit Company settings first!"));
    return;
  }
  foreach (QString k, keys) {
    companyData.insert(k, config->value(k).toString());
  }
  config->endGroup();
}

void DeliveryNote::constructHeader() {
  QTextBlockFormat block;
  block.setAlignment(Qt::AlignCenter);
  QTextCursor cursor = header->textCursor();
  cursor.setCharFormat(headerFormat());
  cursor.insertBlock(block);
  QString str = companyData.value("name");
  cursor.insertText(str.replace("#", "\n"));
  cursor.atEnd();
  int w = header->size().width();
  int fh = cursor.blockCharFormat().font().pointSize();
  int lc = header->document()->lineCount();
  int h = (fh * (lc + 1));
  header->resize(QSize(w, h));
  header->update();
  header->setMaximumHeight(h);
  header->setReadOnly(true);
  header->document()->setModified(true);
}

void DeliveryNote::constructAddress() {
  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(30);
  QTextTable *table = cursor.insertTable(2, 2, format);
  table->setObjectName("address_table");
  // Company Address
  QTextCharFormat cellFormat;
  QFont font(smallFont);
  font.setUnderline(true);
  font.setPointSize(8);
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
  QTextTableCell tc00 = table->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  QString addr(companyData.value("shortname"));
  addr.append(" - ");
  addr.append(companyData.value("street"));
  addr.append(" - ");
  addr.append(companyData.value("location"));
  cursor.insertText(addr);
  // Delivey note
  QTextTableCell tc01 = table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(tr("Delivey note"));
  // Costumer Address
  QTextTableCell tc10 = table->cellAt(1, 0);
  tc10.setFormat(normalFormat());
  cursor = tc10.firstCursorPosition();
  cursor.insertText(p_costumerAddress);
  // Delivery Infos
  QTextTableCell tc11 = table->cellAt(1, 1);
  tc11.setFormat(normalFormat());
  cursor = tc11.firstCursorPosition();
  cursor.insertText(tr("Order-ID:") + " ");
  cursor.insertText(p_orderId);
  cursor.insertText("\n");
  cursor.insertText(tr("Costumer-ID:") + " ");
  cursor.insertText(p_costumerId);
  cursor.insertText("\n");
  cursor.insertText(tr("Delivery-ID:") + " ");
  cursor.insertText(deliveryNumber());
  cursor.insertText("\n");
  body->document()->setModified(true);
}

void DeliveryNote::constructBody() {
  constructAddress();

  QTextCursor cursor = body->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(30);
  m_articleTable = cursor.insertTable(1, 3, format);
  m_articleTable->setObjectName("article_table");

  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setTopBorderBrush(Qt::SolidPattern);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  cellFormat.setBottomBorder(1);
  cellFormat.setBottomBorderBrush(Qt::SolidPattern);
  cellFormat.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  QTextTableCell ce00 = m_articleTable->cellAt(0, 0);
  cursor = ce00.firstCursorPosition();
  ce00.setFormat(cellFormat);
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Article-ID"));

  QTextTableCell ce01 = m_articleTable->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Designation"));

  QTextTableCell ce02 = m_articleTable->cellAt(0, 2);
  ce02.setFormat(cellFormat);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(tr("Quantity"));
  body->document()->setModified(true);
}

void DeliveryNote::constructFooter() {
  QTextCursor cursor = footer->textCursor();
  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setBorderBrush(Qt::black);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  // FOOTER
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(0);
  QTextTable *table = cursor.insertTable(1, 2, format);
  table->setObjectName("footer_table");

  QTextTableCell ce00 = table->cellAt(0, 0);
  ce00.setFormat(cellFormat);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(smallFormat());
  QString name = companyData.value("name");
  cursor.insertText(name.replace("#", " ") + "\n");
  cursor.insertText(companyData.value("street") + " ");
  cursor.insertText(companyData.value("location") + "\n");
  cursor.insertText(tr("email") + ": ");
  cursor.insertText(companyData.value("email") + "\n");
  cursor.insertText(tr("phone") + ": ");
  cursor.insertText(companyData.value("phone") + "\n");
  cursor.insertText(tr("fax") + ": ");
  cursor.insertText(companyData.value("fax") + "\n");

  QTextTableCell ce01 = table->cellAt(0, 1);
  ce01.setFormat(cellFormat);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(smallFormat());
  cursor.insertText(companyData.value("bank") + "\n");
  cursor.insertText("SWIFT-BIC: ");
  cursor.insertText(companyData.value("bicswift") + "\n");
  cursor.insertText("IBAN: ");
  cursor.insertText(companyData.value("iban") + "\n");
  cursor.insertText(tr("tax number") + ": ");
  cursor.insertText(companyData.value("taxnumber") + "\n");

  cursor = footer->textCursor();
  cursor.atEnd();

  int w = footer->size().width();
  int h = (footer->document()->lineCount() * footer->cursorRect().height());
  footer->resize(QSize(w, h));
  footer->update();
  footer->setMaximumHeight(h);
  footer->setReadOnly(true);
  footer->document()->setModified(true);
}

void DeliveryNote::insertArticle(const QString &articleid,
                                 const QString &designation,
                                 const QString &quantity) {
  int row = m_articleTable->rows();
  m_articleTable->insertRows(row, 1);

  QTextCursor cursor = body->textCursor();
  QTextTableCell ce00 = m_articleTable->cellAt(row, 0);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(articleid);

  QTextTableCell ce01 = m_articleTable->cellAt(row, 1);
  cursor = ce01.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(designation);

  QTextTableCell ce02 = m_articleTable->cellAt(row, 2);
  cursor = ce02.firstCursorPosition();
  cursor.setCharFormat(normalFormat());
  cursor.insertText(quantity);
  body->document()->setModified(true);
}

QTextDocument *DeliveryNote::docHeader() { return header->document(); }

const QString DeliveryNote::htmlHeader() {
  return docHeader()->toHtml(defaultCodec());
}

QTextDocument *DeliveryNote::docBody() { return body->document(); }

const QString DeliveryNote::htmlBody() {
  return docBody()->toHtml(defaultCodec());
}

QTextDocument *DeliveryNote::docFooter() { return footer->document(); }

const QString DeliveryNote::htmlFooter() {
  return docFooter()->toHtml(defaultCodec());
}

void DeliveryNote::printDocument(QPrinter *printer) {
  QRect pageRect = pageLayout().fullRectPoints();
  int documentWidth = pageRect.size().width();
  int bodyHeight = pageRect.size().height();

  QString css("* { color: black; }");
  css.append("p, li { white-space: pre-wrap; }");

  QTextDocument *header = docHeader();
  header->setDefaultStyleSheet(css);
  header->setHtml(htmlHeader());
  header->setPageSize(QSizeF(documentWidth, header->size().height()));
  bodyHeight -= header->size().height();

  QTextDocument *footer = docFooter();
  footer->setDefaultStyleSheet(css);
  footer->setHtml(htmlFooter());
  footer->setPageSize(QSizeF(documentWidth, footer->size().height()));
  bodyHeight -= footer->size().height();
  bodyHeight -= 100;

  QTextDocument *body = docBody();
  body->setDefaultStyleSheet(css);
  body->setHtml(htmlBody());
  body->setPageSize(QSizeF(documentWidth, bodyHeight));
  body->setModified(true);

  QRect headerRect = QRect(QPoint(0, 0), header->pageSize().toSize());
  QRect bodycoRect = QRect(QPoint(0, 0), body->pageSize().toSize());
  QRect footerRect = QRect(QPoint(0, 0), footer->pageSize().toSize());

  QByteArray buffer;
  QFile fp(config->value("targets/attachment").toString());
  if (fp.open(QIODevice::ReadOnly)) {
    while (!fp.atEnd()) {
      buffer += fp.readLine();
      ;
    }
  }
  fp.close();
  QImage attachment = QImage::fromData(buffer, "PNG");

  QPainter painter;
  painter.begin(printer);
  if (!attachment.isNull()) {
    painter.translate(0, 0);
    painter.setOpacity(0.3);
    painter.drawImage(QPoint(0, 0), attachment);
    painter.setOpacity(1.0);
  }

  int pY = 0;
  painter.translate(0, pY);
  header->drawContents(&painter, headerRect);
  pY += headerRect.height();
  painter.translate(0, pY);
  body->drawContents(&painter, bodycoRect);
  pY += bodycoRect.height();
  painter.translate(0, pY);
  footer->drawContents(&painter, footerRect);
  pY += footerRect.height();
  painter.translate(0, pY);
  painter.end();
}

void DeliveryNote::saveDeliveryNote() {
  QVariant dest = config->value("targets/deliverynotes", QDir::homePath());
  QFileDialog *dialog = new QFileDialog(this, tr("save pdf"), dest.toString());
  dialog->selectMimeTypeFilter("application/pdf");
  dialog->setNameFilter(tr("PDF Files") + " (*.pdf *.PDF)");
  dialog->setDefaultSuffix(".pdf");
  dialog->selectFile(deliveryNumber() + ".pdf");
  dialog->setAcceptMode(QFileDialog::AcceptSave);
  if (dialog->exec() == QDialog::Accepted) {
    QStringList list = dialog->selectedFiles();
    qDebug() << Q_FUNC_INFO << list.first();
    //    if (saveAsPDF(list.first()))
    //      accept();
  }
}

void DeliveryNote::printDeliveryNote() {
  QPrinter *printer = new QPrinter(QPrinter::PrinterResolution);
  QString dest = QDir::tempPath();
  dest.append(QDir::separator());
  dest.append(deliveryNumber());
  dest.append(".pdf");
  printer->setOutputFileName(dest);
  printer->setPageLayout(pageLayout());
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPaperSource(QPrinter::FormSource);
  printer->setPageMargins(page_margins);
  QPrintDialog *dialog = new QPrintDialog(printer, this);
  connect(dialog, SIGNAL(accepted(QPrinter *)), this,
          SLOT(printDocument(QPrinter *)));
  if (dialog->exec() == QDialog::Accepted) {
    accepted();
  }

  // QPrintPreviewDialog *preview = new QPrintPreviewDialog(printer, this);
  // connect(preview, SIGNAL(paintRequested(QPrinter *)), this,
  //         SLOT(printDocument(QPrinter *)));
  // if (preview->exec() == QDialog::Accepted) {
  //   accepted();
  // }
}

int DeliveryNote::warningMessageBox(const QString &txt) {
  return QMessageBox::warning(this, tr("Delivery note"), txt, QMessageBox::Ok);
}

const QString DeliveryNote::deliveryNumber() {
  QString f;
  QDate date = QDate::currentDate();
  f.append(QString::number(date.year()));
  f.append(QString::number(date.dayOfYear()));
  f.append(p_orderId);
  f.append(p_costumerId);
  return f;
}

void DeliveryNote::setCostumerAddress(const QString &address) {
  if (address.isEmpty()) {
    qWarning("empty costumer address");
    return;
  }
  p_costumerAddress = address;
}

void DeliveryNote::setDelivery(int &orderId, int &costumerId) {
  if (orderId < 1) {
    warningMessageBox(tr("There is no Order-Id to generate this delivery!"));
    return;
  }
  p_orderId = QString::number(orderId);

  if (costumerId < 1) {
    warningMessageBox(tr("There is no Costumer Id to generate this delivery!"));
    return;
  }
  p_costumerId = QString::number(costumerId);
}

int DeliveryNote::exec(const QList<Delivery> &list) {
  if (p_orderId < 1) {
    qFatal("you must call setDelivery() before exec!");
    return 1;
  } else if (p_costumerId < 1) {
    qFatal("you must call setDelivery() before exec!");
    return 1;
  } else if (p_costumerAddress.isEmpty()) {
    warningMessageBox(tr("<p>Costumer Address is empty!</p>"));
    return 1;
  }

  constructHeader();
  constructFooter();
  constructBody();

  QListIterator<Delivery> it(list);
  while (it.hasNext()) {
    Delivery d = it.next();
    insertArticle(d.articleid, d.designation, d.quantity);
  }
  body->setReadOnly(true);

  return Printing::exec();
}
