// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerremoteactions.h"
#include "booklookerrequester.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QImageReader>
#include <QJsonDocument>
#include <QJsonValue>
#include <QVBoxLayout>
#include <QtSql>

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

Bl_PageWidget::Bl_PageWidget(QWidget *parent) : QWidget{parent} {}

void Bl_PageWidget::setModified(bool b) {
  modified = b;
  emit hasModified(b);
}

bool Bl_PageWidget::isModified() { return modified; }

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
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_status"));
  obj.insert("type", QJsonValue());
  emit sendAction(obj);
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        obj.insert("value", QJsonValue(rb->objectName()));
        emit sendAction(obj);
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
  m_apply = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(m_apply);
  setLayout(layout);

  connect(m_apply, SIGNAL(clicked()), this, SLOT(prepareAction()));
}

void Bl_CancelPage::prepareAction() {
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_cancel"));
  obj.insert("type", QJsonValue());
  obj.insert("value", QJsonValue());
  emit sendAction(obj);
}

Bl_EMailPage::Bl_EMailPage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_create_email");
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *lb = new QLabel(tr("Currently not Supported!"), this);
  layout->addWidget(lb, Qt::AlignCenter);
  setLayout(layout);

  // connect(m_apply, SIGNAL(clicked()), this, SLOT(prepareAction()));
}

void Bl_EMailPage::prepareAction() {
  QJsonObject obj;
  obj.insert("action", QJsonValue("post_email"));
  obj.insert("type", QJsonValue());
  obj.insert("value", QJsonValue());
  emit sendAction(obj);
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
  m_message = new QTextEdit(this);
  layout->addWidget(m_message); // #1
  m_apply = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(m_apply); // #2
  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_type, SIGNAL(currentIndexChanged(int)), this,
          SLOT(messageTypeChanged(int)));
  connect(m_apply, SIGNAL(clicked()), this, SLOT(prepareAction()));
}

void Bl_MessagePage::messageTypeChanged(int index) {
  if (index == 0)
    return;

  QString dbName;
  foreach (QString n, QSqlDatabase::connectionNames()) {
    if (n.contains(qApp->applicationName())) {
      dbName = n;
    }
  }
  if (dbName.isEmpty()) {
    emit sendNotes(tr("No Database Connection found!"));
    return;
  }

  QSqlDatabase db = QSqlDatabase::database(dbName);
  if (db.isValid()) {
    qDebug() << Q_FUNC_INFO << dbName;
  }

  m_message->clear();
  QString type = m_type->itemData(index, Qt::UserRole).toString();
  if (type == "PAYMENT_INFORMATION") {
    m_message->setPlainText("@PAYMENT_INFORMATION@");
  } else if (type == "PAYMENT_REMINDER") {
    m_message->setPlainText("@PAYMENT_REMINDER@");
  } else {
    m_message->setPlainText("@SHIPPING_NOTICE@");
  }
}

void Bl_MessagePage::prepareAction() {
  int index = m_type->currentIndex();
  if (index == 0) {
    emit sendNotes(tr("No Message type selected!"));
    m_type->setFocus();
    return;
  }
  QString type = m_type->itemData(index, Qt::UserRole).toString();
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_message"));
  obj.insert("type", QJsonValue(type));
  QString txt = m_message->toPlainText();
  txt.replace("'", "´");
  txt = txt.trimmed();
  obj.insert("value", QJsonValue(txt));
  emit sendAction(obj);
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
  btn_email->setEnabled(false);
  btn_email->setToolTip(tr("Currently not Supported!"));
  // Senden einer E-Mail an den Käufer.
  // btn_email->setToolTip(tr("Sending eMail to Purchaser."));
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

BooklookerRemoteActions::BooklookerRemoteActions(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(QSize(400, 400));
  setWindowTitle("Booklooker [*]");
  setContentsMargins(5, 0, 5, 10);

  m_requester = new BooklookerRequester(this);

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
  m_stackedWidget->insertWidget(page++, m_statusPage);

  m_emailPage = new Bl_EMailPage(this); // #2
  m_stackedWidget->insertWidget(page++, m_emailPage);

  m_cancelPage = new Bl_CancelPage(this); // #3
  m_stackedWidget->insertWidget(page++, m_cancelPage);

  m_messagePage = new Bl_MessagePage(this); // #4
  m_stackedWidget->insertWidget(page++, m_messagePage);
  // END

  // BEGIN SIGNALS
  connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
          SLOT(setCurrentIndex(int)));

  connect(m_statusPage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_statusPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_emailPage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_emailPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_cancelPage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_cancelPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_messagePage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_messagePage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));

  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void BooklookerRemoteActions::prepareAction(const QJsonObject &jsObj) {
  // qDebug() << Q_FUNC_INFO << p_orderId << jsObj;
  QString action = jsObj.value("action").toString();
  QString type = jsObj.value("type").toString();
  QString value = jsObj.value("value").toString();
  if (action == "order_status") {
    m_requester->queryUpdateOrderStatus(p_orderId, value);
  } else if (action == "order_cancel") {
    m_requester->queryUpdateOrderCancel(p_orderId);
  } else if (action == "email") {
    qInfo("TODO: function eMail to purchaser");
  } else if (action == "order_message") {
    m_requester->queryPushMessage(p_orderId, type, value);
  } else {
    qWarning("Unknown action in: %s", Q_FUNC_INFO);
  }
}

void BooklookerRemoteActions::pushMessage(const QString &msg) {
  m_statusBar->showMessage(msg, (1000 * 6));
}

int BooklookerRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
