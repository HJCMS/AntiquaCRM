// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundintroduction.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QStringList>

RefundIntroduction::RefundIntroduction(QWidget *parent)
    : RefundingPage{parent} {
  setObjectName("refund_introduction");

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

  // Information text for the first page of refunding.
  const QString _tr = tr(
      "<p>A return is only necessary if the order was already settled in the "
      "previous month!</p><p>If the order is not paid or settled, a simple "
      "cancellation is sufficient!</p><p>The return will be booked back in the "
      "current month, the statement from the previous month will not be "
      "changed.</p> <p><b>Important:</b> In the case of a return, the article "
      "must be inserted manually into the system.</p><p>Before you continue, "
      "please make sure that this order has already been booked!</p>");

  m_label = new QLabel(_tr, this);
  m_label->setWordWrap(true);
  m_label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  layout->addWidget(m_label);

  const QString _btn = tr("If you are sure, you can continue now.");
  btn_ready = new QPushButton(_btn, parent);
  btn_ready->setIcon(AntiquaCRM::antiquaIcon("dialog-ok-apply"));
  layout->addWidget(btn_ready);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_ready, SIGNAL(clicked()), SIGNAL(sendForward()));
}

bool RefundIntroduction::addDataRecord(qint64 id) {
  Q_UNUSED(id);
  return false;
}
