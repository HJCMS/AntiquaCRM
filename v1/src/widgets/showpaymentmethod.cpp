// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "showpaymentmethod.h"
#include "paymentmethodselecter.h"

ShowPaymentMethod::ShowPaymentMethod(QWidget *parent) : InputEdit{parent} {
  m_text = new QLabel(this);
  m_layout->addWidget(m_text);
  loadDataset();
  setRequired(false);
}

void ShowPaymentMethod::loadDataset() {
  p_data = PaymentMethodSelecter::getMethodes();
  if (p_data.size() > 0) {
    m_text->setText(p_data.value(AntiquaCRM::PAYMENT_NOT_SET));
    p_changed = AntiquaCRM::PAYMENT_NOT_SET;
  }
}

void ShowPaymentMethod::dataChanged(int) { /* unused */
}

void ShowPaymentMethod::reset() { m_text->clear(); }

void ShowPaymentMethod::setValue(const QVariant &val) {
  AntiquaCRM::PaymentMethod set =
      static_cast<AntiquaCRM::PaymentMethod>(val.toInt());

  QMapIterator<AntiquaCRM::PaymentMethod, QString> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (set == it.key()) {
      m_text->setText(it.value());
      p_changed = set;
      break;
    }
  }
}

void ShowPaymentMethod::setFocus() { /* unused */
}

void ShowPaymentMethod::setProperties(const QSqlField &field) {
  Q_UNUSED(field);
  setRequired(false);
}

const QVariant ShowPaymentMethod::value() { return p_changed; }

bool ShowPaymentMethod::isValid() { return true; }

void ShowPaymentMethod::setInfo(const QString &info) {
  m_text->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString ShowPaymentMethod::info() { return m_text->toolTip(); }

const QString ShowPaymentMethod::notes() { return QString("Payment methods!"); }
