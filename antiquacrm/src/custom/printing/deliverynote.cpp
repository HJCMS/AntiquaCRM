// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverynote.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "texteditor.h"

#include <QMessageBox>
#include <QPrintPreviewDialog>

DeliveryNote::DeliveryNote(QWidget *parent) : Printing{parent} {
  setObjectName("printing_delivery_note");
  setWindowTitle(tr("Delivery note"));

  config = new ApplSettings(this);

  printButton->setIcon(myIcon("printer"));
  connect(printButton, SIGNAL(clicked()), this, SLOT(openPrintPreview()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

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
  QTextCursor cursor = editor->textCursor();
  cursor.setCharFormat(headerFormat());
  cursor.insertBlock(block);
  cursor.insertText(companyData.value("name"));
  cursor.setCharFormat(normalFormat());
  cursor.insertText("\n");
}

void DeliveryNote::constructAddressBody() {
  QTextCursor cursor = editor->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(20);
  m_addressTable = cursor.insertTable(2, 2, format);
  // Company Address
  QTextCharFormat cellFormat;
  QFont font(smallFont);
  font.setUnderline(true);
  font.setPointSize(8);
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
  QTextTableCell tc00 = m_addressTable->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  QString addr(companyData.value("shortname"));
  addr.append(" - ");
  addr.append(companyData.value("street"));
  addr.append(" - ");
  addr.append(companyData.value("location"));
  cursor.insertText(addr);
  // Delivey note
  QTextTableCell tc01 = m_addressTable->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(tr("Delivey note"));
  // Costumer Address
  QTextTableCell tc10 = m_addressTable->cellAt(1, 0);
  tc10.setFormat(normalFormat());
  cursor = tc10.firstCursorPosition();
  cursor.insertText(p_costumerAddress);
  // Delivery Infos
  QTextTableCell tc11 = m_addressTable->cellAt(1, 1);
  tc11.setFormat(normalFormat());
  cursor = tc11.firstCursorPosition();
  cursor.insertText(tr("Order-ID:") + " ");
  cursor.insertText(p_orderId);
  cursor.insertText("\n");
  cursor.insertText(tr("Costumer-ID:") + " ");
  cursor.insertText(p_costumerId);
  cursor.insertText("\n");
}

void DeliveryNote::constructBody() {
  QTextCursor cursor = editor->textCursor();
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(20);
  m_articleTable = cursor.insertTable(1, 3, format);

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
}

void DeliveryNote::constructFooter() {
  QTextCursor cursor = editor->textCursor();
  // BEGIN Erstelle Info Zelle
  int row = m_articleTable->rows();
  m_articleTable->insertRows(row, 1);
  p_infoCell = m_articleTable->cellAt(row, 1);
  // END

  QTextTableCellFormat cellFormat;
  cellFormat.setTopBorder(1);
  cellFormat.setBorderBrush(Qt::black);
  cellFormat.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);

  // FOOTER
  QTextTableFormat format = tableFormat();
  format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
  format.setTopMargin(0);
  QTextTable *table = cursor.insertTable(1, 2, format);

  QTextTableCell ce00 = table->cellAt(0, 0);
  ce00.setFormat(cellFormat);
  cursor = ce00.firstCursorPosition();
  cursor.setCharFormat(smallFormat());
  cursor.insertText(companyData.value("name") + "\n");
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

  // :-) i know...
  int max = editor->size().height();
  max -= (editor->cursorRect().height() * 2);
  cursor = p_infoCell.firstCursorPosition();
  int bl = pageSize().rectPoints().bottomLeft().ry();
  for (int i = 0; i < 50; i++) {
    if (max > editor->document()->size().height()) {
      cursor.insertText("\n");
    } else {
      break;
    }
  }
  editor->setReadOnly(true);
}

void DeliveryNote::insertArticle(const QString &articleid,
                                 const QString &designation,
                                 const QString &quantity) {
  int row = m_articleTable->rows();
  m_articleTable->insertRows(row, 1);

  QTextCursor cursor = editor->textCursor();
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
}

void DeliveryNote::printDocument(QPrinter *printer) {
  if (printer != nullptr)
    editor->print(printer);
}

void DeliveryNote::openPrintPreview() {
  QPrinter *printer = new QPrinter(QPrinter::HighResolution);
  printer->setPageLayout(pageLayout());
  printer->setColorMode(QPrinter::GrayScale);
  printer->setPrintRange(QPrinter::CurrentPage);
  printer->setFullPage(true);
  printer->setPageMargins(QMarginsF(0,0,0,0));

  QPrintPreviewDialog *preview = new QPrintPreviewDialog(printer, this);
  connect(preview, SIGNAL(paintRequested(QPrinter *)), this,
          SLOT(printDocument(QPrinter *)));

  preview->exec();
}

int DeliveryNote::warningMessageBox(const QString &txt) {
  return QMessageBox::warning(this, tr("Delivery note"), txt, QMessageBox::Ok);
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

  constructAddressBody();

  constructBody();

  QListIterator<Delivery> it(list);
  while (it.hasNext()) {
    Delivery d = it.next();
    insertArticle(d.articleid, d.designation, d.quantity);
  }

  constructFooter();

  return Printing::exec();
}
