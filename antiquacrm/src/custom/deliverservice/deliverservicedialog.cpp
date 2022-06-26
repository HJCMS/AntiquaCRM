// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicedialog.h"
#include "deliveritem.h"
#include "deliverservicelist.h"
#include "deliverservice.h"

#include <QtCore>
#include <QtWidgets>

DeliverServiceDialog::DeliverServiceDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("deliver_service_dialog");
  setWindowTitle(tr("Deliver Services"));
  setSizeGripEnabled(true);
  setMinimumSize(QSize(600, 400));

  m_sql = new HJCMS::SqlCore(this);

  int row = 0;
  QVBoxLayout *layout = new QVBoxLayout(this);

  m_splitter = new QSplitter(this);
  layout->addWidget(m_splitter);
  layout->setStretch(row++, 1);

  m_list = new DeliverServiceList(m_splitter);
  m_splitter->insertWidget(0, m_list);
  m_splitter->setStretchFactor(0, 20);

  QScrollArea *m_srollArea = new QScrollArea(m_splitter);
  m_srollArea->setWidgetResizable(true);
  m_view = new DeliverService(m_srollArea);
  m_srollArea->setWidget(m_view);

  m_splitter->insertWidget(1, m_srollArea);
  m_splitter->setStretchFactor(1, 80);

  m_buttonBox = new QDialogButtonBox(this);
  layout->addWidget(m_buttonBox);
  layout->setStretch(row++, 0);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  layout->setStretch(row++, 0);

  setLayout(layout);

  connect(m_list, SIGNAL(deliverServiceChanged(int)), this,
          SLOT(queryDeliverServices(int)));
  connect(m_list, SIGNAL(deliverPackageClicked(int, const QString &)), this,
          SLOT(queryServicePackage(int, const QString &)));
}

void DeliverServiceDialog::initItemsTreeView() {
  QString sql("SELECT d_id,d_name,d_website FROM ref_delivery_service;");
  QSqlQuery q = m_sql->query(sql);
  while (q.next()) {
    int id = q.value("d_id").toInt();
    if (id >= 1) {
      m_list->addDeliverService(id, q.value("d_name").toString());
    }
  }
}

void DeliverServiceDialog::queryDeliverServices(int id) {
  QString sql("SELECT * FROM ref_delivery_cost");
  sql.append(" WHERE d_srv=" + QString::number(id));
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

void DeliverServiceDialog::queryServicePackage(int id, const QString &name) {
  qDebug() << Q_FUNC_INFO << id << name;
  return;
  QString sql("SELECT * FROM ref_delivery_service");
  sql.append(" LEFT JOIN ref_delivery_cost ON d_srv=d_id");
  sql.append(" WHERE d_id=" + QString::number(id) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QSqlRecord r = q.record();
    QMap<QString, QVariant> entry;
    int test = 0;
    while (q.next()) {
      if (test > 2)
        break;

      for (int f = 0; f < r.count(); f++) {
        entry.insert(r.fieldName(f), q.value(f));
      }

      test++;
      m_view->addDeliverServiceEntry(entry);
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
}

int DeliverServiceDialog::exec() {
  initItemsTreeView();
  return QDialog::exec();
}
