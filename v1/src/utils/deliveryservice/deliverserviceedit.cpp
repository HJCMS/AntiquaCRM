// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverserviceedit.h"
#include "deliveritem.h"
#include "deliverservicebox.h"

#include <QDebug>
#include <QFrame>
#include <QGridLayout>

DeliverServiceEdit::DeliverServiceEdit(QWidget *parent) : QWidget{parent} {
  setObjectName("deliver_service_view");

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  d_srv = new DeliverServiceBox(this);
  d_srv->setObjectName("d_srv");
  d_srv->setStatusTip(tr("Delivery Service"));
  layout->addWidget(d_srv, row, 0, 1, 1);

  d_international = new BoolBox(this);
  d_international->setObjectName("d_international");
  d_international->setInfo(tr("International"));
  d_international->setToolTip(
      tr("If selected then it is a International Package."));
  layout->addWidget(d_international, row, 1, 1, 1);

  d_default = new BoolBox(this);
  d_default->setObjectName("d_default");
  d_default->setInfo(tr("Default"));
  d_default->setToolTip(tr("Set this Package as default"));
  d_default->setWhatsThis(tr("Set default Package in this List."));
  layout->addWidget(d_default, row, 2, 1, 1);

  d_cid = new IntSpinBox(this);
  d_cid->setObjectName("d_cid");
  d_cid->setRequired(true);
  d_cid->setEnabled(false);
  layout->addWidget(d_cid, row, 3, 1, 1);

  d_price = new PriceEdit(this);
  d_price->setObjectName("d_price");
  d_price->setInfo(tr("Price"));
  d_price->setMinimum(0.00);
  d_price->setRequired(true);
  layout->addWidget(d_price, row++, 4, 1, 1, Qt::AlignLeft);
  layout->setColumnStretch(4, 1);

  layout->addWidget(info(tr("Package")), row, 0, 1, 1);
  d_class = new LineEdit(this);
  d_class->setObjectName("d_class");
  d_class->setToolTip(tr("Service Packagename"));
  d_class->setRequired(true);
  layout->addWidget(d_class, row++, 1, 1, 4);

  layout->addWidget(info(tr("Packageclass")), row, 0, 1, 1);
  d_definition = new LineEdit(this);
  d_definition->setObjectName("d_definition");
  d_definition->setToolTip(tr("Package Definition"));
  d_definition->setRequired(true);
  layout->addWidget(d_definition, row++, 1, 1, 4);

  layout->addWidget(info(tr("Description")), row, 0, 1, 1);
  d_description = new LineEdit(this);
  d_description->setObjectName("d_description");
  d_description->setToolTip(tr("Package Description"));
  d_description->setRequired(true);
  layout->addWidget(d_description, row++, 1, 1, 4);

  layout->addWidget(info(tr("Infopage")), row, 0, 1, 1);
  d_infopage = new EditInfoLabel(this);
  d_infopage->setObjectName("d_infopage");
  d_infopage->setInfo(tr("Package Website Infolink"));
  layout->addWidget(d_infopage, row++, 1, 1, 4);

  layout->addWidget(info(tr("Website")), row, 0, 1, 1);
  d_website = new EditInfoLabel(this);
  d_website->setObjectName("d_website");
  d_website->setInfo(tr("Service Website"));
  layout->addWidget(d_website, row++, 1, 1, 4);

  layout->addWidget(info(tr("Last changed")), row, 0, 1, 1);
  d_changed = new AntiquaDateInfo(this);
  d_changed->setObjectName("d_changed");
  d_changed->setToolTip(tr("Modification date"));
  layout->addWidget(d_changed, row++, 1, 1, 4);

  QFrame *infoBox = new QFrame(this);
  QVBoxLayout *infoLayout = new QVBoxLayout(infoBox);
  QLabel *infoLabel = new QLabel(this);
  infoLabel->setTextFormat(Qt::RichText);
  QString info("<b>Hints</b><ul>");
  QString n1 = tr("The entry currently being edited can be saved!");
  info.append("<li>" + n1 + "</li>");
  QString n2 = tr("All text entries must be set and can't empty!");
  info.append("<li>" + n2 + "</li>");
  QString n3 = tr("Only one Item can set as Default!");
  info.append("<li>" + n3 + "</li>");
  info.append("</ul>");
  infoLabel->setText(info);
  infoLayout->addWidget(infoLabel, Qt::AlignLeft);
  infoBox->setLayout(infoLayout);
  layout->addWidget(infoBox, row++, 0, 1, 4, Qt::AlignLeft);

  layout->setRowStretch(row, 1);
  setLayout(layout);

  d_srv->initDeliverServices();
}

QLabel *DeliverServiceEdit::info(const QString &str) {
  QLabel *lb = new QLabel(this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  lb->setText(str + ":");
  return lb;
}

void DeliverServiceEdit::clearFields() {
  foreach (InputEdit *w, findChildren<InputEdit *>()) {
    w->reset();
  }
  d_changed->setValue(QDateTime::currentDateTime());
  d_srv->setCurrentIndex(1);
  d_cid->setValue(0);
}

void DeliverServiceEdit::createSubEntry(int id) {
  clearFields();
  d_srv->setCurrentIndex(id);
  d_srv->setFocus();
  d_cid->setValue(0);
}

void DeliverServiceEdit::createNewEntry() {
  clearFields();
  d_srv->setFocus();
}

void DeliverServiceEdit::setServiceBoxFocus() { d_srv->setFocus(); }

const DeliverItem DeliverServiceEdit::getSaveData() {
  int id = d_srv->currentIndex();
  QString name = d_class->value().toString();
  DeliverItem info(id, name);

  if (id < 1 && name.isEmpty()) {
    emit message(tr("No valid Dataset present"));
    return info;
  }

  // Is it an INSERT or Update ?
  if (d_cid->value().toInt() > 0) {
    info.setPrimaryKey(d_cid->value().toInt());
  }
  info.setDefinition(d_definition->value().toString());
  info.setInternational(d_international->value().toBool());
  info.setDefault(d_default->value().toBool());
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
    if (it.key() == "d_srv") {
      d_srv->setCurrentIndex(it.value().toInt());
      continue;
    }

    InputEdit *i = findChild<InputEdit *>(it.key());
    if (i != nullptr) {
      i->setValue(it.value());
    }
  }
}
