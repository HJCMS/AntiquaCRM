// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aprintdialog.h"
#include "aprintingformat.h"
#include "aprintingtexteditor.h"

#include <QLayout>

namespace AntiquaCRM {

APrintDialog::APrintDialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(600, 400);

  config = new AntiquaCRM::ASettings(this);
  format = new AntiquaCRM::APrintingFormat(config);

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

  m_toolBar = new QToolBar(this);
  layout->addWidget(m_toolBar);

  QScrollArea *m_scrollArea = new QScrollArea(this);
  m_scrollArea->setWidgetResizable(true);
  layout->addWidget(m_scrollArea);

  printPage = new QWidget(m_scrollArea);
  printPage->setObjectName("printing_area");
  printPage->setContentsMargins(0, 0, 0, 0);
  printPage->setStyleSheet("border:none;");

  QRectF pageRect = QPageSize(QPageSize::A4).rect(QPageSize::Point);
  printPage->setFixedSize(pageRect.toRect().size());

  QVBoxLayout *editorsLayout = new QVBoxLayout(printPage);
  editorsLayout->setContentsMargins(0, 0, 0, 0);
  editorsLayout->setSpacing(0);

  header = new AntiquaCRM::APrintingTextEditor(printPage);
  header->setObjectName("printing_header");
  header->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  editorsLayout->addWidget(header);

  body = new AntiquaCRM::APrintingTextEditor(printPage);
  body->setObjectName("printing_body");
  editorsLayout->addWidget(body);

  footer = new AntiquaCRM::APrintingTextEditor(printPage);
  footer->setObjectName("printing_footer");
  footer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  editorsLayout->addWidget(footer);

  editorsLayout->setStretchFactor(header, 25);
  editorsLayout->setStretchFactor(body, 50);
  editorsLayout->setStretchFactor(footer, 15);
  printPage->setLayout(editorsLayout);
  m_scrollArea->setWidget(printPage);

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->addButton(tr("Setup"), QDialogButtonBox::ActionRole);
  m_buttonBox->addButton(QDialogButtonBox::Apply);
  m_buttonBox->addButton(QDialogButtonBox::Close);
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);
}

APrintDialog::~APrintDialog() {
  // TODO
  format->deleteLater();
}

QTextFrame *APrintDialog::textFrame(QTextDocument *document) {
  QTextFrame *frame = new QTextFrame(document);
  return frame;
}

bool APrintDialog::constructHeader() {
  Q_CHECK_PTR(header);
  QFont font = format->getFont("print_font_header");
  QTextBlockFormat block;
  block.setAlignment(Qt::AlignCenter);
  QTextCursor cursor = header->textCursor();
  cursor.setCharFormat(format->charFormat(font));

  QString title = format->companyData("COMPANY_PRINTING_HEADER");
  foreach (QString line, title.split("\n")) {
    cursor.insertBlock(block);
    cursor.insertText(line);
    cursor.atEnd();
  }

  QFontMetricsF fm(font);
  int w = header->size().width();
  int h = qRound(fm.height() * (header->document()->lineCount() + 1));
  header->resize(QSize(w, h));
  header->update();
  header->setMaximumHeight(h);
  header->setReadOnly(true);
  header->document()->setModified(true);
  return true;
}

QTextTable *APrintDialog::subjectTable(const QString &subject) {
  Q_CHECK_PTR(body);
  QTextCursor cursor = body->textCursor();
  QTextTable *m_table = cursor.insertTable(2, 2, format->tableFormat());

  QTextCharFormat cellFormat;
  QFont font(format->getFont("print_font_small"));
  font.setUnderline(true);
  font.setPointSize(8);
  cellFormat.setFont(font);
  cellFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);

  QTextTableCell tc00 = m_table->cellAt(0, 0);
  tc00.setFormat(cellFormat);
  cursor = tc00.firstCursorPosition();
  QString addr(format->companyData("COMPANY_SHORTNAME"));
  addr.append(" - ");
  addr.append(format->companyData("COMPANY_STREET"));
  addr.append(" - ");
  addr.append(format->companyData("COMPANY_LOCATION"));
  cursor.insertText(addr);

  // Subject
  QTextTableCell tc01 = m_table->cellAt(0, 1);
  tc01.setFormat(cellFormat);
  cursor = tc01.firstCursorPosition();
  cursor.insertText(subject);

  return m_table;
}

int APrintDialog::exec() {
  qWarning("Do not use a Printer dialog without option set!");
  return QDialog::Rejected;
}

} // namespace AntiquaCRM
