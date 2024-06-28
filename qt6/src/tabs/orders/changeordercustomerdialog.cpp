// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "changeordercustomerdialog.h"

ChangeOrderCustomerDialog::ChangeOrderCustomerDialog(QWidget* parent)
    : AntiquaCRM::ADialog{parent} {
  setWindowTitle(tr("Change Customer for Order"));
  setMinimumWidth(600);
  m_mainWidget = new QStackedWidget(this);
  layout->addWidget(m_mainWidget);

  QWidget* m_page1 = new QWidget(m_mainWidget);
  QVBoxLayout* m_page1Layout = new QVBoxLayout(m_page1);
  m_page1Layout->addWidget(warnPage());
  QPushButton* btn_next = new QPushButton(tr("Understood"), m_page1);
  btn_next->setIcon(AntiquaCRM::antiquaIcon("dialog-ok"));
  m_page1Layout->addWidget(btn_next);
  m_page1->setLayout(m_page1Layout);
  m_mainWidget->addWidget(m_page1);

  // signals
  connect(btn_apply, SIGNAL(clicked()), SLOT(confirmd()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
}

QLabel* ChangeOrderCustomerDialog::warnPage() const {
  QString _txt;
  _txt += tr(
      "<h4>Important Warning about this function.</h4>"
      "Why is it a bad idea, to change customer data in existing orders?<p>Customer data included "
      "in provider orders and wil synchronized by Antiqua CRM Customers data.</p><p>But "
      "exactly here is a problem, when there are several service providers whose customer data "
      "collides with that of AntiquaCRM.</p><p>Only when this problem exist, this function will "
      "help to fix it.</p><p><b>Please aware what will done when Customer "
      "changed:</b><ul><li>Change customer data in orders may result in service provider "
      "asynchronous data.<li><li>All Indexes for this order will changed.</li><li>Existing Address "
      "data in invoices and deliveries go broken must regenerated.</li></ul>");

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

void ChangeOrderCustomerDialog::confirmd() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

QDialog::DialogCode ChangeOrderCustomerDialog::start(qint64 id) {
  qDebug() << Q_FUNC_INFO << id;
  return static_cast<QDialog::DialogCode>(QDialog::exec());
}
