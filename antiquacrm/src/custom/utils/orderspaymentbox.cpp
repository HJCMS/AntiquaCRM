#include "orderspaymentbox.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

OrdersPaymentBox::OrdersPaymentBox(QWidget *parent) : UtilsMain{parent} {
  setObjectName("o_order_status");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  m_box = new QComboBox(this);
  m_box->setEditable(false);
  m_box->setDuplicatesEnabled(false);
  m_box->setInsertPolicy(QComboBox::NoInsert);
  m_box->insertItem(0, tr("waiting for payment"), Qt::UserRole);
  m_box->insertItem(1, tr("payment received"), Qt::UserRole);
  layout->addWidget(m_box);

  setModified(false);
  setRequired(false);

  setLayout(layout);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(indexChanged(int)));
}

void OrdersPaymentBox::indexChanged(int i) {
  paymentStatus = ((i == 0) ? false : true);
  setModified(true);
}

void OrdersPaymentBox::setValue(const QVariant &val) {
  paymentStatus = val.toBool();
  m_box->setCurrentIndex(((paymentStatus) ? 1 : 0));
}

void OrdersPaymentBox::reset() {
  m_box->setCurrentIndex(0);
  paymentStatus = false;
  setModified(false);
}

void OrdersPaymentBox::setFocus() { m_box->setFocus(); }

int OrdersPaymentBox::findIndex(const QString &search) {
  return m_box->findText(search, Qt::MatchExactly);
}

const QVariant OrdersPaymentBox::value() { return paymentStatus; }

bool OrdersPaymentBox::isValid() { return true; }

void OrdersPaymentBox::setInfo(const QString &info) {
  m_info->setText(info);
  m_box->setToolTip(info);
}

const QString OrdersPaymentBox::info() { return m_box->toolTip(); }

const QString OrdersPaymentBox::notes() {
  return tr("Payment Status is required");
}

PaymentStatusDialog::PaymentStatusDialog(const QString &search, QWidget *parent)
    : QDialog{parent} {
  setObjectName("payment_status_dialog");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Paymentstatus"));
  setMinimumWidth(300);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *info = new QLabel(this);
  info->setText(tr("Order payment status."));
  layout->addWidget(info);

  m_paymentBox = new OrdersPaymentBox(this);
  layout->addWidget(m_paymentBox);
  int i = m_paymentBox->findIndex(search);
  if (i > 0)
    m_paymentBox->setValue(i);

  QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
  layout->addWidget(btn);

  setLayout(layout);

  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
}

void PaymentStatusDialog::setStatus(bool b) { m_paymentBox->setValue(b); }

bool PaymentStatusDialog::status() { return m_paymentBox->value().toBool(); }
