#include "orderspaymentbox.h"

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

OrdersPaymentBox::OrdersPaymentBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("o_order_status");
  setEditable(false);
  setDuplicatesEnabled(false);
  setInsertPolicy(QComboBox::NoInsert);
  insertItem(0, tr("waiting for payment"), Qt::UserRole);
  insertItem(1, tr("payment received"), Qt::UserRole);
  connect(this, SIGNAL(currentIndexChanged(int)), this,
          SLOT(indexChanged(int)));
}

void OrdersPaymentBox::indexChanged(int i) {
  paymentStatus = ((i == 0) ? false : true);
  emit paymentStatusChanged(true);
}

void OrdersPaymentBox::setPaymentStatus(bool b) {
  paymentStatus = b;
  setCurrentIndex(((b) ? 1 : 0));
  emit paymentStatusChanged(true);
}

bool OrdersPaymentBox::getPaymentStatus() {
  return ((currentIndex() == 0) ? false : true);
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

  m_box = new OrdersPaymentBox(this);
  layout->addWidget(m_box);
  int i = m_box->findText(search, Qt::MatchExactly);
  if (i > 0)
    m_box->setCurrentIndex(i);

  QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
  layout->addWidget(btn);

  setLayout(layout);

  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
}

void PaymentStatusDialog::setStatus(bool b) { m_box->setPaymentStatus(b); }

bool PaymentStatusDialog::status() { return m_box->getPaymentStatus(); }
