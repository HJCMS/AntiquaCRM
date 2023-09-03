// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebookssellerstatus.h"

#include <QRadioButton>
#include <QVBoxLayout>

AbeBooksSellerStatus::AbeBooksSellerStatus(QWidget *parent) : QWidget{parent} {
  setWhatsThis(tr("Change Current order status from AbeBooks."));
  QVBoxLayout *layout = new QVBoxLayout(this);
  QRadioButton *m_rb2 = new QRadioButton(tr("Will ship the item"), this);
  m_rb2->setObjectName("shipped");
  layout->addWidget(m_rb2);
  QRadioButton *m_rb3 = new QRadioButton(tr("Reject this article order"), this);
  m_rb3->setObjectName("rejected");
  layout->addWidget(m_rb3);
  QRadioButton *m_rb4 = new QRadioButton(tr("Already sold the item"), this);
  m_rb4->setObjectName("previouslySold");
  layout->addWidget(m_rb4);
  QRadioButton *m_rb5 =
      new QRadioButton(tr("Buyers credit card was declined."), this);
  m_rb5->setObjectName("creditCardDeclined");
  layout->addWidget(m_rb5);
  QRadioButton *m_rb6 =
      new QRadioButton(tr("Seller Direct order accepted"), this);
  m_rb6->setObjectName("availabilityConfirmed");
  layout->addWidget(m_rb6);
  layout->addStretch();
  setLayout(layout);

  connect(m_rb2, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb3, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb4, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb5, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
  connect(m_rb6, SIGNAL(toggled(bool)), SIGNAL(sendSelectionModified(bool)));
}
