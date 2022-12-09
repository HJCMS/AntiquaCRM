// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicedialog.h"
#include "deliveritem.h"
#include "deliverserviceedit.h"
#include "deliverservicelist.h"

#include <QDebug>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>

DeliverServiceDialog::DeliverServiceDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("deliver_service_dialog");
  setWindowTitle(tr("Deliver Services") + "[*]");
  setSizeGripEnabled(true);
  setMinimumSize(QSize(600, 400));

  m_sql = new AntiquaCRM::ASqlCore(this);

  int row = 0;
  QVBoxLayout *layout = new QVBoxLayout(this);

  m_splitter = new QSplitter(this);
  layout->addWidget(m_splitter);
  layout->setStretch(row++, 1);

  m_list = new DeliverServiceList(m_splitter);
  m_splitter->insertWidget(0, m_list);
  m_splitter->setStretchFactor(0, 70);

  QScrollArea *m_srollArea = new QScrollArea(m_splitter);
  m_srollArea->setWidgetResizable(true);
  m_edit = new DeliverServiceEdit(m_srollArea);
  m_srollArea->setWidget(m_edit);

  m_splitter->insertWidget(1, m_srollArea);
  m_splitter->setStretchFactor(1, 30);

  m_buttonBox = new QDialogButtonBox(this);
  m_buttonBox->setStandardButtons(QDialogButtonBox::Save |
                                  QDialogButtonBox::Close);

  ac_create =
      m_buttonBox->addButton(tr("Create"), QDialogButtonBox::ActionRole);
  ac_create->setIcon(QIcon(":icons/action_add.png"));

  QPushButton *btn_save = m_buttonBox->button(QDialogButtonBox::Save);
  btn_save->setIcon(QIcon("://icons/action_save.png"));
  QPushButton *btn_close = m_buttonBox->button(QDialogButtonBox::Close);
  btn_close->setIcon(QIcon("://icons/action_quit.png"));

  layout->addWidget(m_buttonBox);
  layout->setStretch(row++, 0);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  layout->setStretch(row++, 0);

  setLayout(layout);

  connect(m_list, SIGNAL(deliverServiceChanged(int)), this,
          SLOT(queryDeliverServices(int)));

  connect(m_list, SIGNAL(deliverPackageClicked(int, const QJsonObject &)), this,
          SLOT(queryServicePackage(int, const QJsonObject &)));

  connect(m_list, SIGNAL(removeDeliveryPackage(const QJsonObject &)), this,
          SLOT(deleteDeliveryPackage(const QJsonObject &)));

  connect(m_edit, SIGNAL(message(const QString &)), this,
          SLOT(messanger(const QString &)));

  connect(m_list, SIGNAL(createDeliveryService(int)), m_edit,
          SLOT(createSubEntry(int)));
  connect(ac_create, SIGNAL(clicked()), m_edit, SLOT(createNewEntry()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(saveCurrent()));
}

void DeliverServiceDialog::initItemsTreeView() {
  m_list->clear();
  QString sql("SELECT d_id,d_name,d_website FROM ref_delivery_service;");
  QSqlQuery q = m_sql->query(sql);
  while (q.next()) {
    int id = q.value("d_id").toInt();
    if (id >= 1) {
      m_list->addDeliverService(id, q.value("d_name").toString());
    }
  }
  m_list->expandAll();
}

void DeliverServiceDialog::messanger(const QString &msg) {
  m_statusBar->showMessage(msg, (1000 * 5));
}

void DeliverServiceDialog::deleteDeliveryPackage(const QJsonObject &obj) {
  // Referenze mit: uniq_delivery_cost
  QString sql("DELETE FROM ref_delivery_cost WHERE");
  sql.append(" d_srv=" + QString::number(obj["d_srv"].toInt()) + "");
  QString d_class = obj["d_class"].toString();
  sql.append(" AND d_class='" + d_class + "'");
  QString d_description = obj["d_description"].toString();
  sql.append(" AND d_description='" + d_description + "'");
  QString d_definition = obj["d_definition"].toString();
  sql.append(" AND d_definition='" + d_definition + "'");
  qreal d_price = obj["d_price"].toDouble();
  sql.append(" AND d_price=" + QString::number(d_price) + ";");
  // qDebug() << Q_FUNC_INFO << sql;
  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    initItemsTreeView();
  } else {
    QString info = m_sql->lastError();
    QMessageBox *errDialog =
        new QMessageBox(QMessageBox::Critical, tr("Database Error"), info,
                        QMessageBox::Ok, this);
    errDialog->setInformativeText(sql);
    errDialog->exec();
  }
}

void DeliverServiceDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    // Key press enter or return will ignored!
    return;
  }
  QDialog::keyPressEvent(e);
}

bool DeliverServiceDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void DeliverServiceDialog::saveCurrent() {
  DeliverItem data = m_edit->getSaveData();
  int d_cid = data.primaryKey();
  // FOREIGN KEY check
  if (data.index() < 1) {
    messanger(tr("Invalid") + " SQL:'FOREIGN KEY'");
    m_edit->setServiceBoxFocus();
    return;
  }
  QString d_srv = QString::number(data.index());
  QString d_class = data.name();
  QString d_definition = data.definition();
  QString d_international = (data.international() ? "true" : "false");
  QString d_default = (data.isDefault() ? "true" : "false");
  QString d_description = data.description();
  QString d_price = QString::number(data.price(), 'f', 2);

  QString sql;
  if (d_cid > 0) {
    // SQL UPDATE
    sql.append("UPDATE ref_delivery_cost SET");
    sql.append(" d_srv=");
    sql.append(d_srv);
    sql.append(", d_class='");
    sql.append(d_class);
    sql.append("', d_definition='");
    sql.append(d_definition);
    sql.append("', d_international=");
    sql.append(d_international);
    sql.append(", d_description='");
    sql.append(d_description);
    sql.append("', d_price=");
    sql.append(d_price);
    sql.append(", d_default=" + d_default);
    sql.append(" WHERE d_cid=");
    sql.append(QString::number(d_cid));
    sql.append(";");
  } else {
    // SQL INSERT
    sql.append("INSERT INTO ref_delivery_cost (");
    sql.append("d_srv,d_class,d_definition,");
    sql.append("d_international,d_description,");
    sql.append("d_price) VALUES (");
    sql.append(d_srv + ",");
    sql.append("'" + d_class + "',");
    sql.append("'" + d_definition + "',");
    sql.append(d_international + ",");
    sql.append("'" + d_description + "',");
    sql.append(d_price);
    sql.append(");");
  }
  // qDebug() << "SQL:" << sql;
  if (data.isDefault()) {
    // Restore before Update for Unique
    QString u_sql("UPDATE ref_delivery_cost SET d_default=false");
    u_sql.append(" WHERE d_cid!=" + QString::number(d_cid) + ";");
    m_sql->query(u_sql);
  }

  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    initItemsTreeView();
  } else {
    QString info = m_sql->lastError();
    QMessageBox *errDialog =
        new QMessageBox(QMessageBox::Critical, tr("Database Error"), info,
                        QMessageBox::Ok, this);
    errDialog->setInformativeText(sql);
    errDialog->exec();
  }
}

void DeliverServiceDialog::queryDeliverServices(int id) {
  QString sql("SELECT * FROM ref_delivery_cost");
  sql.append(" WHERE d_srv=" + QString::number(id) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString name = q.value("d_class").toString();
      DeliverItem info(id, name);
      info.setDefinition(q.value("d_definition").toString());
      info.setInternational(q.value("d_international").toBool());
      info.setDescription(q.value("d_description").toString());
      info.setPrice(q.value("d_price").toDouble());
      info.setChanged(q.value("d_changed").toDateTime());
      m_list->addDeliverPackage(id, info);
    }
  }
}

void DeliverServiceDialog::queryServicePackage(int id, const QJsonObject &obj) {
  QString sql("SELECT * FROM ref_delivery_service");
  sql.append(" LEFT JOIN ref_delivery_cost ON d_srv=d_id");
  sql.append(" WHERE d_id=" + QString::number(id));
  sql.append(" AND d_class='" + obj["d_class"].toString() + "'");
  sql.append(" AND d_definition='" + obj["d_definition"].toString() + "'");
  sql.append(" AND d_description='" + obj["d_description"].toString() + "';");

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QSqlRecord r = q.record();
    QMap<QString, QVariant> entry;
    while (q.next()) {
      for (int f = 0; f < r.count(); f++) {
        entry.insert(r.fieldName(f), q.value(f));
      }
    }
    m_edit->addDeliverServiceEntry(entry);
  }
}

int DeliverServiceDialog::exec() {
  initItemsTreeView();
  return QDialog::exec();
}
