// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printinglayout.h"

#include <QLayout>

BorderPrintInput::BorderPrintInput(QWidget *parent, qreal max)
    : InputEdit{parent} {
  m_box = new QDoubleSpinBox(this);
  m_box->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_box->setRange(0, max);
  m_box->setSuffix(" px");
  m_box->setGroupSeparatorShown(true);
  m_layout->addWidget(m_box);

  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setRange(0, max);
  m_slider->setTickInterval(1);
  m_layout->addWidget(m_slider);

  setModified(false);
  setRequired(false);

  connect(m_slider, SIGNAL(valueChanged(int)), SLOT(borderChanged(int)));
}

void BorderPrintInput::borderChanged(int i) {
  if (i % 2 & 1)
    return;

  m_box->setValue(static_cast<qreal>(i));
}

void BorderPrintInput::reset() {
  m_box->setValue(0.00);
  m_slider->setValue(0);
}

void BorderPrintInput::setValue(const QVariant &val) {
  m_slider->setValue(val.toInt());
  m_box->setValue(val.toDouble());
}

void BorderPrintInput::setFocus() { m_slider->setFocus(); }

void BorderPrintInput::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (!field.defaultValue().isNull() && m_box->value() == 0.00) {
    setValue(field.defaultValue());
  }
}

const QVariant BorderPrintInput::value() { return m_box->value(); }

bool BorderPrintInput::isValid() { return true; }

void BorderPrintInput::setInfo(const QString &info) {
  if (!info.isEmpty()) {
    m_label->setVisible(true);
    m_label->setMinimumWidth(50);
    m_label->setText(info);
  }
  m_slider->setToolTip(info);
}

const QString BorderPrintInput::info() {
  if (m_label->isVisible())
    return m_label->text();
  else
    return m_slider->toolTip();
}

const QString BorderPrintInput::notes() {
  return tr("A valid border set is required!");
}

PrintingLayout::PrintingLayout(QWidget *parent)
    : QGroupBox{parent}, p_top{0.0}, p_bottom{0.0} {
  setTitle(tr("Printer page layout margins."));

  QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

  QGridLayout *inpLayout = new QGridLayout;
  m_left = new BorderPrintInput(this);
  m_left->setObjectName("table_margin_left");
  m_left->setToolTip(tr("Document distance left."));
  inpLayout->addWidget(label(tr("Left")), 0, 0, 1, 1);
  inpLayout->addWidget(m_left, 0, 1, 1, 1);

  m_right = new BorderPrintInput(this);
  m_right->setObjectName("table_margin_right");
  m_right->setToolTip(tr("Document distance right."));
  inpLayout->addWidget(label(tr("Right")), 1, 0, 1, 1);
  inpLayout->addWidget(m_right, 1, 1, 1, 1);

  inpLayout->setRowStretch(2, 1);

  m_subject = new BorderPrintInput(this, 200);
  m_subject->setObjectName("subject_position");
  m_subject->setToolTip(tr("Distance from header to the recipient address."));
  inpLayout->addWidget(label(tr("Subject")), 3, 0, 1, 1);
  inpLayout->addWidget(m_subject, 3, 1, 1, 1);

  m_body = new BorderPrintInput(this, 200);
  m_body->setObjectName("body_position");
  m_body->setToolTip(tr("Distance from Document subject to content."));
  inpLayout->addWidget(label(tr("Body")), 4, 0, 1, 1);
  inpLayout->addWidget(m_body, 4, 1, 1, 1);

  m_recipient = new QSlider(Qt::Horizontal, this);
  m_recipient->setObjectName("recipient_padding_top");
  m_recipient->setToolTip(tr("Recipient Address distance top."));
  m_recipient->setRange(1, 10);
  inpLayout->addWidget(label(tr("Recipient")), 5, 0, 1, 1);
  inpLayout->addWidget(m_recipient, 5, 1, 1, 1);

  boxLayout->addLayout(inpLayout);

  QString info = tr("In this area you can change the margins of the print area "
                    "in invoices and delivery notes. This does not affect the "
                    "aspect ratio of the documents.");
  QLabel *m_info = new QLabel(this);
  m_info->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_info->setMargin(5);
  m_info->setWordWrap(true);
  m_info->setText(info);
  boxLayout->addWidget(m_info);

  boxLayout->setStretchFactor(inpLayout, 60);
  boxLayout->setStretchFactor(m_info, 40);
  setLayout(boxLayout);
}

QLabel *PrintingLayout::label(const QString &title) {
  QLabel *lb = new QLabel(title + ":", this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

void PrintingLayout::setValue(const QMarginsF &m) {
  m_left->setValue(m.left());
  p_top = m.top();
  m_right->setValue(m.right());
  p_bottom = m.bottom();
}

void PrintingLayout::setValue(qreal l, qreal t, qreal r, qreal b) {
  m_left->setValue(l);
  p_top = t;
  m_right->setValue(r);
  p_bottom = b;
}

const QMarginsF PrintingLayout::value() {
  qreal left = m_left->value().toReal();
  qreal right = m_right->value().toReal();
  return QMarginsF(left, p_top, right, p_bottom);
}

void PrintingLayout::setSubjectPosition(int i) { m_subject->setValue(i); }

int PrintingLayout::getSubjectPosition() { return m_subject->value().toInt(); }

void PrintingLayout::setBodyPosition(int i) { m_body->setValue(i); }

int PrintingLayout::getBodyPosition() { return m_body->value().toInt(); }

void PrintingLayout::setRecipientPosition(qreal r) { m_recipient->setValue(r); }

qreal PrintingLayout::getRecipientPosition() {
  return static_cast<qreal>(m_recipient->value());
}
