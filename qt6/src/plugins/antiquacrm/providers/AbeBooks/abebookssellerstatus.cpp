// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebookssellerstatus.h"

#include <QFrame>
#include <QList>
#include <QRadioButton>
#include <QVBoxLayout>

AbeBooksSellerStatus::AbeBooksSellerStatus(QWidget *parent) : QWidget{parent} {
  setWhatsThis(tr("Change Current order status from AbeBooks."));
  setContentsMargins(0, 0, 0, 0);

  int _row = 0;
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_buyerInfo = new QLabel(this);
  m_buyerInfo->setTextFormat(Qt::RichText);
  m_buyerInfo->setOpenExternalLinks(true);
  layout->insertWidget(_row++, m_buyerInfo);

  QFrame *m_frame = new QFrame(this);
  m_frame->setFrameStyle(QFrame::NoFrame);
  m_frame->setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *statusLayout = new QVBoxLayout(m_frame);
  statusLayout->setContentsMargins(0, 0, 0, 0);

  QRadioButton *m_rb2 = new QRadioButton(tr("Will ship the item"), this);
  m_rb2->setObjectName("shipped");
  statusLayout->addWidget(m_rb2);

  QRadioButton *m_rb3 = new QRadioButton(tr("Reject this article order"), this);
  m_rb3->setObjectName("rejected");
  statusLayout->addWidget(m_rb3);

  QRadioButton *m_rb4 = new QRadioButton(tr("Already sold the item"), this);
  m_rb4->setObjectName("previouslySold");
  statusLayout->addWidget(m_rb4);

  QRadioButton *m_rb5 =
      new QRadioButton(tr("Buyers credit card was declined."), this);
  m_rb5->setObjectName("creditCardDeclined");
  statusLayout->addWidget(m_rb5);

  QRadioButton *m_rb6 =
      new QRadioButton(tr("Seller Direct order accepted"), this);
  m_rb6->setObjectName("availabilityConfirmed");
  statusLayout->addWidget(m_rb6);

  statusLayout->addStretch();
  m_frame->setLayout(statusLayout);
  layout->insertWidget(_row++, m_frame);
  setLayout(layout);

  connect(m_rb2, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb3, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb4, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb5, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb6, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
}

void AbeBooksSellerStatus::setBuyerInfo(const QString &txt) {
  if (txt.contains("https:")) {
    m_buyerInfo->setToolTip(tr("shipment manifest"));
  } else {
    m_buyerInfo->setToolTip(tr("buyer information"));
  }
  m_buyerInfo->setText(txt);
}

const QString AbeBooksSellerStatus::getStatus() {
  QString _status;
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        _status = rb->objectName();
        break;
      }
    }
  }
  return _status;
}
