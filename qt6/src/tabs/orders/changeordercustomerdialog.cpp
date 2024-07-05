// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "changeordercustomerdialog.h"
#include "changecustomerfind.h"
#include "changecustomerwidget.h"

#include <QMessageBox>

ChangeOrderCustomerDialog::ChangeOrderCustomerDialog(QWidget* parent)
    : AntiquaCRM::ADialog{parent} {
  setWindowTitle(tr("Change Customer for this Order"));
  setMinimumSize(600, 450);

  m_mainWidget = new QStackedWidget(this);
  m_mainWidget->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_mainWidget);

  const QMargins layoutMargins(0, 0, 0, 0);

  // BEGIN:Page1
  QWidget* m_page1 = new QWidget(m_mainWidget);
  m_page1->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout* m_page1Layout = new QVBoxLayout(m_page1);
  m_page1Layout->setContentsMargins(layoutMargins);
  m_page1Layout->addWidget(getPage0());
  QPushButton* btn_next = new QPushButton(tr("Understood"), m_page1);
  btn_next->setIcon(AntiquaCRM::antiquaIcon("dialog-ok"));
  m_page1Layout->addWidget(btn_next);
  m_page1->setLayout(m_page1Layout);
  m_mainWidget->insertWidget(0, m_page1);
  // END:Page1

  // BEGIN:Page2
  QWidget* m_page2 = new QWidget(m_mainWidget);
  m_page2->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout* m_page2Layout = new QVBoxLayout(m_page2);
  m_page2Layout->setContentsMargins(layoutMargins);
  m_findCustomer = new ChangeCustomerFind(m_page2);
  m_page2Layout->addWidget(m_findCustomer);
  m_dataWidget = new ChangeCustomerWidget(m_page2);
  m_page2Layout->addWidget(m_dataWidget);
  m_page2->setLayout(m_page2Layout);
  m_mainWidget->insertWidget(1, m_page2);
  // END:Page2

  m_mainWidget->setCurrentIndex(0);

  btn_apply->setText(tr("Change Customer"));
  btn_apply->setToolTip(windowTitle());

  // signals
  connect(btn_next, SIGNAL(clicked()), SLOT(setPage1()));
  connect(btn_apply, SIGNAL(clicked()), SLOT(apply()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(close()));
  connect(m_findCustomer, SIGNAL(sendCustomerSelected(qint64)), SLOT(setPage2(qint64)));
  connect(m_findCustomer, SIGNAL(sendNotification(QString)), SLOT(statusBarMessage(QString)));
}

QLabel* ChangeOrderCustomerDialog::getPage0() const {
  QString _txt;
  _txt +=
      tr("<div><p>Normally, it is not necessary to change customer data in existing "
         "orders.</p><p>Customer data is included in the provider's orders and synchronized with "
         "the customer data in AntiquaCRM.</p><p>However, sometimes there is a problem when "
         "service provider data differs from the customer data in AntiquaCRM.</p><p>If this "
         "problem exists, this function should help to resolve it.</p><b>Please note what happens "
         "when a customer changes:</b><ul><li>Changing customer data in orders can lead to "
         "asynchronous data with the service providers.</li><li>Existing address data in invoices "
         "and delivery notes islost and must be regenerated.</li></ul></div>");

  QString _css("QLabel {background: transparent; border:none;}");
  QLabel* m_lb = new QLabel(m_mainWidget);
  m_lb->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_lb->setTextInteractionFlags(Qt::TextBrowserInteraction);
  m_lb->setTextFormat(Qt::RichText);
  m_lb->setWordWrap(true);
  m_lb->setStyleSheet(_css);
  m_lb->setText(_txt);
  return m_lb;
}

int ChangeOrderCustomerDialog::exec() {
  return QDialog::exec();
}

void ChangeOrderCustomerDialog::setPage1() {
  m_mainWidget->setCurrentIndex(1);
}

void ChangeOrderCustomerDialog::setPage2(qint64 cid) {
  if (cid < 1)
    return;

  if (m_dataWidget->loadCostumerData(cid)) {
    m_mainWidget->setCurrentIndex(2);
    btn_apply->setToolTip(tr("Apply to this customer!"));
    btn_apply->setEnabled(true);
  } else {
    statusBarMessage(tr("Can not find customer data with this id!"));
  }
}

void ChangeOrderCustomerDialog::apply() {
  QMessageBox* m_ask = new QMessageBox(this);
  m_ask->setWindowTitle(tr("Change Customer to order."));
  m_ask->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  m_ask->setDefaultButton(QMessageBox::Cancel);
  m_ask->setEscapeButton(QMessageBox::Cancel);

  QString _txt = tr("Last question before a new customer is assigned to this order.");
  _txt.append("<br>");
  _txt.append(tr("Press ”Ok” to change customer to this order."));
  m_ask->setText(_txt);

  if (m_ask->exec() == QMessageBox::Ok)
    done(QDialog::Accepted);
}

void ChangeOrderCustomerDialog::close() {
  done(QDialog::Rejected);
}

qint64 ChangeOrderCustomerDialog::start(qint64 cid) {
  if (cid < 1)
    return QDialog::Rejected;

  m_dataWidget->setCustomerId(cid);
  if (exec() == QDialog::Accepted)
    return m_dataWidget->getCustomerId();

  return cid;
}
