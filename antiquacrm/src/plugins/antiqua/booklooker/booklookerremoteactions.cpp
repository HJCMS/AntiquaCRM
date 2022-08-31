// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerremoteactions.h"
#include "applsettings.h"
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
  QDir p = ApplSettings::getDataTarget("icons");
  QFileInfo fileInfo(p, "info.png");
  if (fileInfo.isReadable()) {
    QImageReader reader(fileInfo.filePath());
    QImage img = reader.read();
    if (!img.isNull())
      return QIcon(QPixmap::fromImage(img));
  }
  return QIcon();
}

Bl_StatusPage::Bl_StatusPage(QWidget *parent) : QWidget{parent} {
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

  connect(m_rb2, SIGNAL(toggled(bool)), this, SIGNAL(sendModified(bool)));
  connect(m_rb3, SIGNAL(toggled(bool)), this, SIGNAL(sendModified(bool)));
  connect(m_rb4, SIGNAL(toggled(bool)), this, SIGNAL(sendModified(bool)));
  connect(m_rb5, SIGNAL(toggled(bool)), this, SIGNAL(sendModified(bool)));
  connect(m_rb6, SIGNAL(toggled(bool)), this, SIGNAL(sendModified(bool)));
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

BooklookerRemoteActions::BooklookerRemoteActions(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setWindowTitle("Booklooker [*]");
  setContentsMargins(5, 0, 5, 10);

  m_requester = new BooklookerRequester(this);
  m_requester->setObjectName("bl_remote_actions");

  // BEGIN MainLayout
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);
  m_statusPage = new Bl_StatusPage(this);
  layout->addWidget(m_statusPage);
  layout->setStretch(0, 1);
  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_buttonBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);
  // END

  // BEGIN SIGNALS
  connect(m_requester, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(jsonResponse(const QJsonDocument &)));
  connect(m_statusPage, SIGNAL(sendModified(bool)), this,
          SLOT(setWindowModified(bool)));
  connect(m_statusPage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_statusPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(closeAction()));
  // END
}

void BooklookerRemoteActions::prepareAction(const QJsonObject &jsObj) {
  QString action = jsObj.value("action").toString();
  QString type = jsObj.value("type").toString();
  QString value = jsObj.value("value").toString();
  if (action == "order_status") {
    m_requester->queryUpdateOrderStatus(p_orderId, value);
  } else {
    qWarning("Unknown action in: %s", Q_FUNC_INFO);
  }
}

void BooklookerRemoteActions::pushMessage(const QString &msg) {
  m_statusBar->showMessage(msg, 6000);
}

void BooklookerRemoteActions::jsonResponse(const QJsonDocument &jdoc) {
  if (QJsonValue(jdoc["status"]).toString() == "NOK") {
    QString value = QJsonValue(jdoc["returnValue"]).toString();
    if (value == "INVALID_PARAMETERS") {
      pushMessage(tr("Invalid Parameters"));
    } else if (value == "TOKEN_EXPIRED") {
      pushMessage(tr("Token Expired"));
    } else if (value == "SIX_WEEKS_EXCEEDED") {
      pushMessage(tr("This Order no longer exists!"));
    } else if (value == "NO_ARTICLES_FOUND") {
      pushMessage(tr("Order not found!"));
    }
    qWarning("Booklooker Error: %s", qPrintable(value));
    return;
  }
  QString value = QJsonValue(jdoc["returnValue"]).toString();
  pushMessage(value.trimmed());
}

void BooklookerRemoteActions::closeAction() {
  if (isWindowModified()) {
    pushMessage(tr("Unfinished operations!"));
    setWindowModified(false);
    return;
  }
  reject();
}

void BooklookerRemoteActions::setPurchaser(const QString &person) {
  if (person.isEmpty())
    return;

  p_purchaser = person;
}

int BooklookerRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4) {
    qWarning("BooklookerRemoteActions: orderID rejected.");
    return QDialog::Rejected;
  }

  p_orderId = orderId;
  return QDialog::exec();
}
