// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoftremoteactions.h"
#include "whsoftconfig.h"
#include "whsoftjsonquery.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QJsonValue>
#include <QLayout>
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

/**
 * @brief WHSoftRemoteActions
 */
WHSoftRemoteActions::WHSoftRemoteActions(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setWindowTitle("Buchfreund [*]");
  setWindowIcon(btnIcon());
  setContentsMargins(5, 0, 5, 10);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  // BEGIN MainLayout
  m_mainWidget = new QWidget(this);
  QHBoxLayout *m_mainLayout = new QHBoxLayout(this);
  m_orderId = new QLabel(m_mainWidget);
  m_mainLayout->addWidget(m_orderId);
  m_supplier = new QComboBox(m_mainWidget);
  m_mainLayout->addWidget(m_supplier);
  m_shippingMethod = new QLabel(m_mainWidget);
  m_shippingMethod->setText(tr("Package"));
  m_mainLayout->addWidget(m_shippingMethod);
  m_trackingNumber = new QLineEdit(m_mainWidget);
  m_mainLayout->addWidget(m_trackingNumber);
  m_deliveryDate = new QDateTimeEdit(m_mainWidget);
  m_mainLayout->addWidget(m_deliveryDate);
  m_mainLayout->addStretch(1);
  btn_status = new QPushButton(btnIcon(), tr("Delivery Status"), m_mainWidget);
  // Bestellstatus aktualisieren.
  btn_status->setToolTip(tr("Order Status update."));
  m_mainLayout->addWidget(btn_status);
  // END

  layout->addWidget(m_mainWidget);
  layout->setStretch(0, 1);
  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_buttonBar);
  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);

  // BEGIN WHSoftJSonQuery
  m_requester = new WHSoftJSonQuery(this);
  m_requester->setObjectName("bf_remote_actions");
  // END

  connect(m_requester, SIGNAL(customQueryResponse(const QJsonDocument &)), this,
          SLOT(jsonResponse(const QJsonDocument &)));

  connect(btn_status, SIGNAL(clicked()), this, SLOT(prepareAction()));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
}

bool WHSoftRemoteActions::initDatabaseSetup() {
  QString dbName;
  foreach (QString n, QSqlDatabase::connectionNames()) {
    if (n.contains(qApp->applicationName())) {
      dbName = n;
    }
  }
  if (dbName.isEmpty()) {
    pushMessage(tr("No Database Connection found!"));
    return false;
  }

  QSqlDatabase db = QSqlDatabase::database(dbName);
  if (!db.isValid()) {
    pushMessage(tr("No Database Connection!"));
    return false;
  }

  QString sql("SELECT d_id, d_name FROM ref_delivery_service");
  sql.append(" ORDER BY d_id ASC;");
  QSqlQuery q = db.exec(sql);
  if (q.size() > 0) {
    if (m_supplier->count() >= 1) {
      m_supplier->clear();
    }
    int i = 0;
    while (q.next()) {
      m_supplier->insertItem(i, q.value("d_name").toString());
      m_supplier->setItemData(i, q.value("d_id").toInt(), Qt::UserRole);
      i++;
    }
  }
  return true;
}

void WHSoftRemoteActions::prepareAction() {
  if (m_orderId->text().isEmpty())
    return;

  QJsonObject jo;
  jo.insert("id", QJsonValue(m_orderId->text()));
  jo.insert("versender", QJsonValue(m_supplier->currentText()));
  jo.insert("sendungsart", QJsonValue(m_shippingMethod->text()));
  jo.insert("sendungsnummer", QJsonValue(m_trackingNumber->text()));
  QString dt = m_deliveryDate->dateTime().toString(DATE_FORMAT);
  jo.insert("einlieferungsdatum", QJsonValue(dt));

  qDebug() << Q_FUNC_INFO << "versandbestaetigung" << jo;
  // m_requester->customQuery("versandbestaetigung", QJsonDocument(jo));
}

void WHSoftRemoteActions::pushMessage(const QString &msg) {
  m_statusBar->showMessage(msg, (1000 * 6));
}

void WHSoftRemoteActions::jsonResponse(const QJsonDocument &jdoc) {
  if (QJsonValue(jdoc["error"]).toBool()) {
    qDebug() << Q_FUNC_INFO << jdoc;
    pushMessage(tr("An error has occurred!"));
    return;
  }
  pushMessage(tr("Delivery update success!"));
}

int WHSoftRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  if (!orderId.contains("BF-"))
    return QDialog::Rejected;

  p_orderId = orderId;
  m_orderId->setText(p_orderId);

  if (!initDatabaseSetup())
    return QDialog::Rejected;

  return QDialog::exec();
}
