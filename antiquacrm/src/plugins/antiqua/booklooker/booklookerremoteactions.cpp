// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerremoteactions.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QImageReader>
#include <QVBoxLayout>

static const QIcon btnIcon() {
  QDir dir = QDir::current();
  dir.setPath("icons");
  QFileInfo fileInfo(dir, "info.png");
  if (fileInfo.isReadable()) {
    QImageReader reader(fileInfo.filePath());
    QImage img = reader.read();
    if (!img.isNull())
      return QIcon(QPixmap::fromImage(img));
  }
  return QIcon();
}

Bl_StartPage::Bl_StartPage(QWidget *parent) : QWidget{parent} {
  setObjectName("booklooker_action_start_page");
  QGridLayout *layout = new QGridLayout(this);

  btn_status = new QPushButton(btnIcon(), tr("Status"), this);
  btn_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QFont btn_font = btn_status->font();
  btn_font.setStretch(1);
  btn_status->setFont(btn_font);
  // Bestellstatus aktualisieren.
  btn_status->setToolTip(tr("Order Status update."));
  layout->addWidget(btn_status, 0, 0, 1, 1);

  btn_email = new QPushButton(btnIcon(), tr("eMail"), this);
  btn_email->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_email->setFont(btn_font);
#ifndef ANTIQUA_DEVELOPEMENT
  btn_email->setEnabled(false);
  btn_email->setToolTip(tr("Currently not Supported!"));
#else
  // Senden einer E-Mail an den Käufer.
  btn_email->setToolTip(tr("Sending eMail to Purchaser."));
#endif
  layout->addWidget(btn_email, 0, 1, 1, 1);

  btn_cancel = new QPushButton(btnIcon(), tr("Cancel"), this);
  btn_cancel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_cancel->setFont(btn_font);
  // Stornieren einer kompletten Bestellung.
  btn_cancel->setToolTip(tr("Cancel an entire order."));
  layout->addWidget(btn_cancel, 1, 0, 1, 1);

  btn_message = new QPushButton(btnIcon(), tr("Message"), this);
  btn_message->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_message->setFont(btn_font);
  // Versand einer Nachricht an den Käufer.
  btn_message->setToolTip(tr("Sending a message to the Purchaser."));
  layout->addWidget(btn_message, 1, 1, 1, 1);

  setLayout(layout);

  connect(btn_status, SIGNAL(clicked()), this, SLOT(statusClicked()));
  connect(btn_email, SIGNAL(clicked()), this, SLOT(emailClicked()));
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
  connect(btn_message, SIGNAL(clicked()), this, SLOT(messageClicked()));
}

void Bl_StartPage::statusClicked() { emit sendGotoPage(1); }

void Bl_StartPage::emailClicked() { emit sendGotoPage(2); }

void Bl_StartPage::cancelClicked() { emit sendGotoPage(3); }

void Bl_StartPage::messageClicked() { emit sendGotoPage(4); }

Bl_StatusPage::Bl_StatusPage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_order_status");
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *m_lb = new QLabel(this);
  m_lb->setText(tr("Changes the status of the current order."));
  layout->addWidget(m_lb);

  m_rb1 = new QRadioButton(tr("Provider Status - no disclosures"), this);
  m_rb1->setChecked(true);
  layout->addWidget(m_rb1);

  m_rb2 = new QRadioButton(tr("Wait for Order Payment"), this);
  m_rb2->setObjectName("WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb2);

  m_rb3 = new QRadioButton(tr("Order is ready for Shipment"), this);
  m_rb3->setObjectName("READY_FOR_SHIPMENT");
  layout->addWidget(m_rb3);

  m_rb4 = new QRadioButton(tr("Order shipped, wait for Payment."), this);
  m_rb4->setObjectName("SHIPPED_WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb4);

  m_rb5 = new QRadioButton(tr("Order Shipped and Paid"), this);
  m_rb5->setObjectName("SHIPPED_AND_PAID");
  layout->addWidget(m_rb5);

  m_rb6 = new QRadioButton(tr("Order Buyer no reaction."), this);
  m_rb6->setObjectName("BUYER_NO_REACTION");
  layout->addWidget(m_rb6);

  m_apply = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(m_apply);

  layout->addStretch(1);
  setLayout(layout);

  connect(m_apply, SIGNAL(clicked()), this, SLOT(prepareAction()));
}

void Bl_StatusPage::prepareAction() {
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        emit sendAction("order_status", rb->objectName());
        break;
      }
    }
  }
}

Bl_CancelPage::Bl_CancelPage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_cancel_order");
  QVBoxLayout *layout = new QVBoxLayout(this);
  // order_cancel
  QLabel *lb = new QLabel(this);
  lb->setTextFormat(Qt::RichText);
  QString txt("<b>");
  txt.append(tr("Cancel the currently selected order."));
  txt.append("</b><p>");
  txt.append(tr("A message to the buyer must be sent separately."));
  txt.append("</p>");
  lb->setText(txt);
  layout->addWidget(lb);
  layout->addStretch(1);
  QPushButton *btn = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(btn);
  setLayout(layout);
}

void Bl_CancelPage::prepareAction() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

Bl_EMailPage::Bl_EMailPage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_create_email");
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *lb = new QLabel(tr("Currently not Supported!"), this);
  layout->addWidget(lb, Qt::AlignCenter);
  setLayout(layout);
}

void Bl_EMailPage::prepareAction() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

Bl_MessagePage::Bl_MessagePage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_messanger");
  // order_message
  QVBoxLayout *layout = new QVBoxLayout(this);
  m_type = new QComboBox(this);
  // Bitte wählen Sie zuerst die Versandart aus.
  m_type->addItem(tr("Please select send type first."), QString());
  // PAYMENT_INFORMATION · Zahlungsinformationen senden
  m_type->addItem(tr("Send payment information"), "PAYMENT_INFORMATION");
  // PAYMENT_REMINDER · Zahlungserinnerung senden
  m_type->addItem(tr("Send payment reminder"), "PAYMENT_REMINDER");
  // SHIPPING_NOTICE · Versandmitteilung senden
  m_type->addItem(tr("Send shipping notification"), "SHIPPING_NOTICE");
  layout->addWidget(m_type); // #0
  m_preDefTxt = new QComboBox(this);
  layout->addWidget(m_preDefTxt); // #1
  m_message = new QTextEdit(this);
  layout->addWidget(m_message); // #2
  QPushButton *btn = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(btn); // #3
  layout->setStretch(2, 1);
  setLayout(layout);
}

void Bl_MessagePage::prepareAction() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

BooklookerRemoteActions::BooklookerRemoteActions(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(QSize(400, 400));
  setWindowTitle("Booklooker [*]");
  setContentsMargins(5, 0, 5, 10);

  // BEGIN MainLayout
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);
  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);
  layout->setStretch(0, 1);
  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_buttonBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);
  // END

  // BEGIN Pages
  int page = 0;
  m_startPage = new Bl_StartPage(this);
  m_stackedWidget->insertWidget(page++, m_startPage);

  m_statusPage = new Bl_StatusPage(this); // #1
  connect(m_statusPage, SIGNAL(sendAction(const QString &, const QString &)),
          this, SLOT(prepareAction(const QString &, const QString &)));
  m_stackedWidget->insertWidget(page++, m_statusPage);

  m_emailPage = new Bl_EMailPage(this); // #2
  connect(m_emailPage, SIGNAL(sendAction(const QString &, const QString &)),
          this, SLOT(prepareAction(const QString &, const QString &)));
  m_stackedWidget->insertWidget(page++, m_emailPage);

  m_cancelPage = new Bl_CancelPage(this); // #3
  connect(m_cancelPage, SIGNAL(sendAction(const QString &, const QString &)),
          this, SLOT(prepareAction(const QString &, const QString &)));
  m_stackedWidget->insertWidget(page++, m_cancelPage);

  m_messagePage = new Bl_MessagePage(this); // #4
  connect(m_messagePage, SIGNAL(sendAction(const QString &, const QString &)),
          this, SLOT(prepareAction(const QString &, const QString &)));
  m_stackedWidget->insertWidget(page++, m_messagePage);
  // END

  connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
          SLOT(setCurrentIndex(int)));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
}

void BooklookerRemoteActions::prepareAction(const QString &action,
                                            const QString &value) {
  // order_status,order_cancel,order_message
  qDebug() << Q_FUNC_INFO << p_orderId << action << value;
}

int BooklookerRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
