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

BL_Message::BL_Message() {
  p_type = QString();
  p_title = QString();
  p_body = QString();
}

void BL_Message::setType(const QString &str) { p_type = str; }

const QString BL_Message::getType() { return p_type; }

void BL_Message::setTitle(const QString &str) { p_title = str; }

const QString BL_Message::getTitle() { return p_title; }

void BL_Message::setBody(const QString &str) { p_body = str; }

const QString BL_Message::getBody() { return p_body; }

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

Bl_MessagePage::Bl_MessagePage(QWidget *parent) : Bl_PageWidget{parent} {
  setObjectName("booklooker_messanger");
  // order_message
  QVBoxLayout *layout = new QVBoxLayout(this);
  m_type = new QComboBox(this);
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

const QString Bl_MessagePage::convertTemplate(const QString &body) {
  QString buffer = body.trimmed();
  QMapIterator<QString, QString> it(p_replacements);
  while (it.hasNext()) {
    it.next();
    buffer.replace(it.key(), it.value());
  }
  // Remove duplicates
  buffer.replace(QRegExp("\\bHerr\\s+Herr\\b"),"Herr");
  buffer.replace(QRegExp("\\bFrau\\s+Frau\\b"),"Frau");
  return buffer;
}

void Bl_MessagePage::messageTypeChanged(int index) {
  if (index == 0)
    return;

  BL_Message bl_m = p_messages.value(index);
  QString body = bl_m.getBody();
  if (body.isEmpty())
    return;

  m_message->clear();
  m_message->setPlainText(body);
}

void Bl_MessagePage::prepareAction() {
  int index = m_type->currentIndex();
  if (index == 0) {
    emit sendNotes(tr("No Message type selected!"));
    m_type->setFocus();
    return;
  }
  QString type = m_type->itemData(index, Qt::UserRole).toString();
  type.replace("PROVIDER_","");
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_message"));
  obj.insert("type", QJsonValue(type));
  QString txt = m_message->toPlainText();
  txt.replace("'", "´");
  txt = txt.trimmed();
  obj.insert("value", QJsonValue(txt));
  emit sendAction(obj);
}

void Bl_MessagePage::setPurchaser(QString &person, const QString &orderId) {
  p_replacements.insert("@PROVIDER_PURCHASER@", person);
  p_replacements.insert("@PROVIDER_ORDER_ID@", orderId);
}

bool Bl_MessagePage::initSqlMessages() {
  QString dbName;
  foreach (QString n, QSqlDatabase::connectionNames()) {
    if (n.contains(qApp->applicationName())) {
      dbName = n;
    }
  }
  if (dbName.isEmpty()) {
    qWarning("No Database Connection found!");
    emit sendNotes(tr("No Database Connection found!"));
    return false;
  }

  QSqlDatabase db = QSqlDatabase::database(dbName);
  if (!db.isValid()) {
    qWarning("No Database Connection found!");
    emit sendNotes(tr("No Database Connection!"));
    return false;
  }

  QStringList where_in;
  QStringList types = BooklookerRequester::orderMessageTypes();
  foreach (QString t, types) {
    where_in << "'PROVIDER_" + t + "'";
  }

  QString sql("SELECT * FROM ui_template_body WHERE");
  sql.append(" tb_caller IN (" + where_in.join(",") + ");");

  QSqlQuery q = db.exec(sql);
  if (q.size() > 0) {
    p_messages.clear();
    m_type->addItem(tr("Please select send type first."), QString());
    int position = 1;

    while (q.next()) {
      QString body = q.value("tb_message").toString();
      if (body.isEmpty())
        continue;

      BL_Message bl_p;
      bl_p.setType(q.value("tb_caller").toString());
      bl_p.setTitle(q.value("tb_title").toString());
      body = convertTemplate(body);
      bl_p.setBody(body);

      p_messages.insert(position, bl_p);
      m_type->insertItem(position, bl_p.getTitle(), bl_p.getType());
      position++;
    }
    return true;
  }
  return true;
}

Bl_StartPage::Bl_StartPage(QWidget *parent) : QWidget{parent} {
  setObjectName("booklooker_action_start_page");
  QGridLayout *layout = new QGridLayout(this);

  QIcon icon = btnIcon();
  QSize iconSize(24, 24);

  btn_status = new QPushButton(icon, tr("Status"), this);
  btn_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_status->setIconSize(iconSize);
  btn_status->setMinimumHeight(btn_status->width());
  QFont btn_font = btn_status->font();
  btn_font.setPointSize((btn_font.pointSize() + 2));
  btn_status->setFont(btn_font);
  // Bestellstatus aktualisieren.
  btn_status->setToolTip(tr("Order Status update."));
  layout->addWidget(btn_status, 0, 0, 1, 1);

  btn_message = new QPushButton(icon, tr("Message"), this);
  btn_message->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_message->setFont(btn_font);
  btn_message->setIconSize(iconSize);
  btn_message->setMinimumHeight(btn_message->width());
  // Versand einer Nachricht an den Käufer.
  btn_message->setEnabled(true);
  btn_message->setToolTip(tr("Sending a message to the Purchaser."));
  layout->addWidget(btn_message, 0, 1, 1, 1);

  layout->setRowStretch(1, 1);
  setLayout(layout);

  connect(btn_status, SIGNAL(clicked()), this, SLOT(statusClicked()));
  connect(btn_message, SIGNAL(clicked()), this, SLOT(messageClicked()));
}

void Bl_StartPage::statusClicked() { emit sendGotoPage(1); }

void Bl_StartPage::messageClicked() { emit sendGotoPage(2); }

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

  // Bestellstatus
  m_statusPage = new Bl_StatusPage(this); // #1
  m_stackedWidget->insertWidget(page++, m_statusPage);

  // Nachricht an
  m_messagePage = new Bl_MessagePage(this); // #2
  m_stackedWidget->insertWidget(page++, m_messagePage);
  // END

  // BEGIN SIGNALS
  connect(m_requester, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(jsonResponse(const QJsonDocument &)));
  connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
          SLOT(setCurrentIndex(int)));
  connect(m_statusPage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_statusPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_messagePage, SIGNAL(sendAction(const QJsonObject &)), this,
          SLOT(prepareAction(const QJsonObject &)));
  connect(m_messagePage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void BooklookerRemoteActions::prepareAction(const QJsonObject &jsObj) {
  QString action = jsObj.value("action").toString();
  QString type = jsObj.value("type").toString();
  QString value = jsObj.value("value").toString();
  if (action == "order_status") {
    m_requester->queryUpdateOrderStatus(p_orderId, value);
  } else if (action == "order_message") {
    m_requester->queryPushMessage(p_orderId, type, value);
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

  if (!p_purchaser.isEmpty())
    m_messagePage->setPurchaser(p_purchaser, orderId);

  if (!m_messagePage->initSqlMessages())
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
