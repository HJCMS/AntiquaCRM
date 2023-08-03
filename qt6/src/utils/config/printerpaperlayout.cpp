// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerpaperlayout.h"
#include "printersetborder.h"

#include <QGridLayout>

PrinterPaperLayout::PrinterPaperLayout(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Margins of the page layout."));

  QGridLayout *layout = new QGridLayout(this);
  int row = 0;

  QLabel *info = new QLabel(this);
  info->setIndent(2);
  info->setWordWrap(true);
  info->setText(tr("In this area you can change the margins of the print area "
                   "in invoices and delivery notes. This does not affect the "
                   "aspect ratio of the documents."));
  info->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  layout->addWidget(info, row++, 0, 1, 2);

  // Margin:Left
  layout->addWidget(label(tr("Left")), row, 0, 1, 1);
  QString _tip = tr("General print area. From margin left to content.");
  m_marginLeft = new PrinterSetBorder(_tip, this);
  m_marginLeft->setObjectName("page_margin_left");
  m_marginLeft->setValue(15); // DIN 5008B
  layout->addWidget(m_marginLeft, row++, 1, 1, 1);

  // Margin:Footer
  layout->addWidget(label(tr("Top")), row, 0, 1, 1);
  _tip = tr("General print area. From margin top to content.");
  m_marginTop = new PrinterSetBorder(_tip, this);
  m_marginTop->setObjectName("page_margin_top");
  layout->addWidget(m_marginTop, row++, 1, 1, 1);

  // Margin:Right
  layout->addWidget(label(tr("Right")), row, 0, 1, 1);
  _tip = tr("General print area. From content to margin right.");
  m_marginRight = new PrinterSetBorder(_tip, this);
  m_marginRight->setObjectName("page_margin_right");
  layout->addWidget(m_marginRight, row++, 1, 1, 1);

  // Margin:Bottom
  layout->addWidget(label(tr("Bottom")), row, 0, 1, 1);
  _tip = tr("Distance from Document Footer top bottom.");
  m_marginBottom = new PrinterSetBorder(_tip, this);
  m_marginBottom->setObjectName("page_margin_bottom");
  layout->addWidget(m_marginBottom, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_marginLeft, SIGNAL(updateChanged(bool)),
          SLOT(setInputChanges(bool)));
  connect(m_marginTop, SIGNAL(updateChanged(bool)),
          SLOT(setInputChanges(bool)));
  connect(m_marginRight, SIGNAL(updateChanged(bool)),
          SLOT(setInputChanges(bool)));
  connect(m_marginBottom, SIGNAL(updateChanged(bool)),
          SLOT(setInputChanges(bool)));
}

QLabel *PrinterPaperLayout::label(const QString &title) {
  QLabel *m_lb = new QLabel(this);
  m_lb->setIndent(2);
  m_lb->setText(title + ":");
  m_lb->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  return m_lb;
}

void PrinterPaperLayout::setInputChanges(bool b) {
  setWindowModified(b);
}

void PrinterPaperLayout::loadSection(AntiquaCRM::ASettings *config) {
  QListIterator<PrinterSetBorder *> it(
      findChildren<PrinterSetBorder *>(QString()));

  if (!it.hasNext())
    return;

  while (it.hasNext()) {
    PrinterSetBorder *obj = it.next();
    if (obj == nullptr)
      continue;

    QVariant _var = config->value(obj->objectName(), 20);
    obj->setValue(_var.toDouble());
  }
}

void PrinterPaperLayout::saveSection(AntiquaCRM::ASettings *config) {
  QListIterator<PrinterSetBorder *> it(
      findChildren<PrinterSetBorder *>(QString()));

  if (!it.hasNext())
    return;

  while (it.hasNext()) {
    PrinterSetBorder *obj = it.next();
    if (obj == nullptr)
      continue;

    config->setValue(obj->objectName(), obj->getValue());
  }
}
