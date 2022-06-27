// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverserviceedit.h"
#include "applsettings.h"
#include "deliveritem.h"

#include <QDebug>
#include <QGridLayout>

DeliverServiceEdit::DeliverServiceEdit(QWidget *parent) : QWidget{parent} {
  setObjectName("deliver_service_view");

  ApplSettings cfg;

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  d_srv = new DeliveryService(this);
  d_srv->setObjectName("d_srv");
  d_srv->setInfo(tr("Service"));
  d_srv->setStatusTip(tr("Delivery Service"));
  layout->addWidget(d_srv, row, 0, 1, 1);

  d_international = new BoolBox(this);
  d_international->setObjectName("d_international");
  d_international->setInfo(tr("International"));
  d_international->setToolTip(
      tr("If selected then it is a International Package."));
  layout->addWidget(d_international, row, 1, 1, 1);

  d_price = new PriceEdit(this);
  d_price->setObjectName("d_price");
  d_price->setToolTip(cfg.value("payment/currency").toString());
  d_price->setInfo(tr("costs"));
  d_price->setMinimum(0.10);
  d_price->setRequired(true);
  layout->addWidget(d_price, row++, 2, 1, 1, Qt::AlignLeft);
  layout->setColumnStretch(2, 1);

  layout->addWidget(info(tr("Package")), row, 0, 1, 1);
  d_class = new StrLineEdit(this);
  d_class->setObjectName("d_class");
  d_class->setInfo(tr("Package"));
  d_class->setToolTip(tr("Service Packagename"));
  d_class->setRequired(true);
  layout->addWidget(d_class, row++, 1, 1, 2, Qt::AlignLeft);

  layout->addWidget(info(tr("Definition")), row, 0, 1, 1);
  d_definition = new StrLineEdit(this);
  d_definition->setObjectName("d_definition");
  d_definition->setInfo(tr("Definition"));
  d_definition->setToolTip(tr("Package Definition"));
  d_definition->setRequired(true);
  layout->addWidget(d_definition, row++, 1, 1, 2);

  layout->addWidget(info(tr("Description")), row, 0, 1, 1);
  d_description = new StrLineEdit(this);
  d_description->setObjectName("d_description");
  d_description->setInfo(tr("Description"));
  d_description->setToolTip(tr("Package Description"));
  d_description->setRequired(true);
  layout->addWidget(d_description, row++, 1, 1, 2);

  layout->addWidget(info(tr("Infopage")), row, 0, 1, 1);
  d_infopage = new StrLineEdit(this);
  d_infopage->setObjectName("d_infopage");
  d_infopage->setInfo(tr("Infopage"));
  d_infopage->setToolTip(tr("Package Website Infolink"));
  layout->addWidget(d_infopage, row++, 1, 1, 2);

  layout->addWidget(info(tr("Webseite")), row, 0, 1, 1);
  d_website = new StrLineEdit(this);
  d_website->setObjectName("d_website");
  d_website->setInfo(tr("Webseite"));
  d_website->setToolTip(tr("Service Website"));
  layout->addWidget(d_website, row++, 1, 1, 2);

  d_changed = new DateTimeDisplay(this);
  d_changed->setObjectName("d_changed");
  d_changed->setToolTip(tr("Modification date"));
  layout->addWidget(d_changed, row++, 0, 1, 3);

  layout->setRowStretch(row, 1);
  setLayout(layout);

  d_srv->loadSqlDataset();
}

QLabel *DeliverServiceEdit::info(const QString &str) {
  QLabel *lb = new QLabel(this);
  lb->setAlignment(Qt::AlignRight);
  lb->setText(str + ":");
  return lb;
}

const DeliverItem DeliverServiceEdit::getSaveData() {
  int id = d_srv->value().toInt();
  QString name = d_class->value().toString();
  DeliverItem info(id, name);

  if(id < 1 && name.isEmpty()) {
    emit message(tr("No Dataset present"));
    return info;
  }

  info.setDefinition(d_definition->value().toString());
  info.setInternational(d_international->value().toBool());
  info.setDescription(d_description->value().toString());
  info.setPrice(d_price->value().toDouble());
  info.setChanged(QDateTime::currentDateTime());
  return info;
}

void DeliverServiceEdit::addDeliverServiceEntry(
    const QMap<QString, QVariant> &entry) {
  QMapIterator<QString, QVariant> it(entry);
  while (it.hasNext()) {
    it.next();
    UtilsMain *i = findChild<UtilsMain *>(it.key());
    if (i != nullptr) {
      i->setValue(it.value());
    }
  }
}
