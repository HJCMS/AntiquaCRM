// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordereditor.h"
#include "applsettings.h"
#include "deliverservice.h"
#include "deliverynote.h"
#include "editoractionbar.h"
#include "invoice.h"
#include "mailforwarddialog.h"
#include "myicontheme.h"
#include "ordersitemlist.h"
#include "orderstatements.h"
#include "paymentreminder.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QMessageBox>
#include <QtWidgets>

OrderEditor::OrderEditor(QWidget *parent) : EditorMain{parent} {
  setObjectName("order_editor");
  setWindowTitle(tr("Edit Order"));
  setMinimumHeight(500);

  m_cfg = new ApplSettings(this);

  /**
   * Tabellenfelder welche NICHT bei INSERT/UPDATE
   * benötigt werden aber im Overview enthalten sind.
   */
  ignoreList.clear();
  ignoreList.append("a_modified");
  ignoreList.append("o_since");
  ignoreList.append("o_modified");
  ignoreList.append("o_delivered");
  ignoreList.append("o_provider_order");

  ignoreOnInsert.clear();
  ignoreOnInsert.append("o_invoice_id");
  ignoreOnInsert.append("o_delivery");
  ignoreOnInsert.append("o_delivered");

  Qt::Alignment defaultAlignment = (Qt::AlignRight | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(2, 2, 2, 2);
  mainLayout->setObjectName("mainLayout");

  QScrollArea *m_scroolView = new QScrollArea(this);
  m_scroolView->setObjectName("editor_scroll_view");
  m_scroolView->setWidgetResizable(true);

  QWidget *mainWidget = new QWidget(m_scroolView);
  mainWidget->setObjectName("editor_main_widget");
  mainWidget->setMinimumHeight(600);
  m_scroolView->setWidget(mainWidget);

  QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
  mainWidgetLayout->setObjectName("mainLayout");

  QHBoxLayout *row0 = new QHBoxLayout();
  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order ID"));
  o_id->setRequired(true);
  row0->addWidget(o_id);

  o_invoice_id = new SerialID(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setInfo(tr("Invoice ID"));
  row0->addWidget(o_invoice_id);

  o_payment_status = new OrdersPaymentBox(mainWidget);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setInfo(tr("Payment"));
  o_payment_status->setValue(0);
  row0->addWidget(o_payment_status);

  o_order_status = new OrderStatusBox(mainWidget);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setInfo(tr("Status"));
  o_order_status->setValue(1);
  row0->addWidget(o_order_status);

  row0->addStretch(1);
  mainWidgetLayout->addLayout(row0);

  // Zweite Reihe
  QHBoxLayout *row1 = new QHBoxLayout();
  /**
   * Links
   * Dient nur zur Info die Daten werden mit Tabelle "customers" Verknüpft!
   */
  QWidget *m_customerInfo = new QWidget(mainWidget);
  m_customerInfo->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout *cInfoLayout = new QVBoxLayout(m_customerInfo);
  cInfoLayout->setContentsMargins(2, 2, 2, 2);
  o_customer_id = new SerialID(m_customerInfo);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setInfo(tr("Customer Number") + ":");
  cInfoLayout->addWidget(o_customer_id);
  m_customer_address = new QLabel(m_customerInfo);
  m_customer_address->setObjectName("m_customer_address");
  m_customer_address->setToolTip(tr("Customer Info"));
  m_customer_address->setIndent(10);
  cInfoLayout->addWidget(m_customer_address);
  m_customerInfo->setLayout(cInfoLayout);
  row1->addWidget(m_customerInfo); // links

  /**
   * Rechts
   * Sektionen:
   *  Lieferservice
   *  Dienstleister
   *  Rechnungs Einstellungen
   */
  QTabWidget *tabwidget = new QTabWidget(this);
  tabwidget->setObjectName("row1_tabwidget_right");

  // BEGIN Lieferdienst
  m_deliveryBox = new QWidget(tabwidget);
  tabwidget->addTab(m_deliveryBox, myIcon("spreadsheet"),
                    tr("Delivery Service"));
  QGridLayout *dsLayout = new QGridLayout(m_deliveryBox);
  o_delivery_service = new DeliverService(m_deliveryBox);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInfo(tr("Service"));
  dsLayout->addWidget(o_delivery_service, 0, 0, 1, 3, Qt::AlignLeft);
  o_delivery_send_id = new LineEdit(m_deliveryBox);
  o_delivery_send_id->setObjectName("o_delivery_send_id");
  o_delivery_send_id->setInfo(tr("Parcel Shipment Number"));
  dsLayout->addWidget(o_delivery_send_id, 1, 0, 1, 2);
  o_delivery = new LineEdit(m_deliveryBox);
  o_delivery->setObjectName("o_delivery");
  o_delivery->setInfo(tr("Delivery note number"));
  dsLayout->addWidget(o_delivery, 2, 0, 1, 2);
  QToolButton *btn_gen_deliver = new QToolButton(m_deliveryBox);
  btn_gen_deliver->setObjectName("btn_gen_delivernote_id");
  btn_gen_deliver->setToolTip(tr("Generate delivery note Number"));
  btn_gen_deliver->setIcon(myIcon("folder_public"));
  dsLayout->addWidget(btn_gen_deliver, 2, 2, 1, 1);
  dsLayout->setRowStretch(3, 1);
  m_deliveryBox->setLayout(dsLayout);
  // END

  // BEGIN Rechnungs Einstellungen
  m_billingBox = new QWidget(tabwidget);
  tabwidget->addTab(m_billingBox, myIcon("spreadsheet"), tr("Invoice options"));
  QGridLayout *billingLayout = new QGridLayout(m_billingBox);
  int brow = 0;
  o_vat_country = new EUCountryBox(m_billingBox);
  o_vat_country->setInfo(tr("European Countries"));
  billingLayout->addWidget(o_vat_country, brow++, 0, 1, 2);
  o_vat_included = new BoolBox(m_billingBox);
  o_vat_included->setInfo(tr("vat included"));
  o_vat_included->setToolTip(
      tr("Normally the vat is included in Book articles. Uncheck it to enable "
         "add VAT in Printing Invoice."));
  o_vat_included->setChecked(true);
  billingLayout->addWidget(o_vat_included, brow, 0, 1, 1, Qt::AlignRight);
  o_vat_levels = new QComboBox(m_billingBox);
  int vat1 = m_cfg->value("payment/vat1").toInt();
  int vat2 = m_cfg->value("payment/vat2").toInt();
  o_vat_levels->insertItem(0, QString::number(vat2) + "% " + tr("Reduced"));
  o_vat_levels->setItemData(0, vat2, Qt::UserRole);
  o_vat_levels->insertItem(1, QString::number(vat1) + "% " + tr("Normal"));
  o_vat_levels->setItemData(1, vat1, Qt::UserRole);
  o_vat_levels->insertItem(2, tr("No VAT"));
  o_vat_levels->setItemData(2, 0, Qt::UserRole);
  o_vat_levels->setCurrentIndex(0);
  billingLayout->addWidget(o_vat_levels, brow++, 1, 1, 1);
  o_delivery_add_price = new BoolBox(m_billingBox);
  o_delivery_add_price->setInfo(tr("add delivery package price"));
  o_delivery_add_price->setToolTip(
      tr("add delivery package price to current shipping."));
  o_delivery_add_price->setChecked(false);
  o_delivery_add_price->setRequired(false);
  billingLayout->addWidget(o_delivery_add_price, brow++, 0, 1, 2);

  billingLayout->setRowStretch(brow, 1);
  m_billingBox->setLayout(billingLayout);
  // END

  // BEGIN Dienstleister
  m_providerBox = new QWidget(tabwidget);
  tabwidget->addTab(m_providerBox, myIcon("spreadsheet"), tr("Provider"));
  QGridLayout *providerLayout = new QGridLayout(m_providerBox);
  providerLayout->setColumnStretch(1, 1);
  o_provider_name = new LineEdit(m_providerBox);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setInfo(tr("Provider"));
  o_provider_name->setRequired("true");
  providerLayout->addWidget(o_provider_name, 0, 0, 1, 1);
  providerLayout->addWidget(new QLabel(m_providerBox), 0, 1, 1, 1);
  o_provider_order_id = new LineEdit(m_providerBox);
  o_provider_order_id->setObjectName("o_provider_order_id");
  o_provider_order_id->setInfo(tr("settlement number"));
  o_provider_order_id->setRequired("true");
  providerLayout->addWidget(o_provider_order_id, 1, 0, 1, 1);
  providerLayout->setRowStretch(2, 1);
  m_providerBox->setLayout(providerLayout);
  row1->addWidget(tabwidget);
  // END

  mainWidgetLayout->addLayout(row1);

  // Artikel Einkaufsliste
  mainWidgetLayout->addWidget(
      new QLabel(tr("Current shopping items") + ":", mainWidget));

  m_paymentList = new OrdersItemList(mainWidget);
  m_paymentList->setObjectName("m_paymentList");
  mainWidgetLayout->addWidget(m_paymentList);

  mainWidgetLayout->addStretch(1);
  mainWidget->setLayout(mainWidgetLayout);
  mainLayout->addWidget(m_scroolView);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->setObjectName("m_actionBar");
  m_actionBar->setPrinterMenu(PrinterButton::Delivery | PrinterButton::Invoice |
                              PrinterButton::Reminder);
  m_actionBar->setMailMenu(MailButton::Orders);
  m_actionBar->setViewRestoreButton(false);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // SIGNALS
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *e = *it;
    connect(e, SIGNAL(hasModified(bool)), this, SLOT(setWindowModified(bool)));
  }

  connect(btn_gen_deliver, SIGNAL(clicked()), this,
          SLOT(generateDeliveryNumber()));
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_printDeliveryNote()), this,
          SLOT(openPrinterDeliveryDialog()));
  connect(m_actionBar, SIGNAL(s_printInvoiceNote()), this,
          SLOT(openPrinterInvoiceDialog()));
  connect(m_actionBar, SIGNAL(s_printPaymentReminder()), this,
          SLOT(openPrinterPaymentReminderDialog()));
  connect(m_actionBar, SIGNAL(s_createMailMessage(const QString &)), this,
          SLOT(openEMailDialog(const QString &)));
  connect(m_paymentList, SIGNAL(searchArticle(int)), this,
          SLOT(findArticle(int)));
  connect(m_paymentList, SIGNAL(statusMessage(const QString &)), this,
          SLOT(showMessagePoUp(const QString &)));
  connect(m_paymentList, SIGNAL(hasModified(bool)), this,
          SLOT(setWindowModified(bool)));
  connect(o_customer_id, SIGNAL(s_serialChanged(int)), this,
          SLOT(findCustomer(int)));
  connect(m_paymentList, SIGNAL(askToRemoveRow(int)), this,
          SLOT(findRemoveTableRow(int)));
}

void OrderEditor::setInputList() {
  inputList = m_sql->fields("inventory_orders");
  if (inputList.isEmpty()) {
    qWarning("Customers InputList is Empty!");
  }
  /* Werden manuell gesetzt oder sind hier nicht notwendig! */
  foreach (QString f, ignoreList) {
    inputList.removeOne(f);
  }
}

void OrderEditor::importSqlResult() {
  if (sqlQueryResult.size() < 5)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataField f = sqlQueryResult.at(i);
    setData(f.field(), f.value(), f.isRequired());
  }
  blockSignals(false);

  // Nach Ersteintrag zurück setzen!
  resetModified(inputList);
  m_paymentList->setModified(false);
}

const QHash<QString, QVariant> OrderEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (ignoreList.contains(cur->objectName(), Qt::CaseSensitive))
      continue;

    if (cur->isRequired() && !cur->isValid()) {
      sqlNoticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    // qDebug() << "Orders:" << cur->objectName() << cur->value();
    data.insert(cur->objectName(), cur->value());
  }

  /**
   * Der Eintrag ist in "o_delivery_service" enthalten!
   * Muss deshalb Explizit abgerufen werden!
   * Er darf nicht "-1" sein, was bei nicht Initialisierung der fall ist!
   */
  int package_id = o_delivery_service->getDeliveryPackage();
  if (package_id >= 0) {
    data.insert("o_delivery_package", package_id);
  }

  list.clear();
  return data;
}

void OrderEditor::invalidRelationship() {
  QString body("<p>");
  body.append(tr("Failed relationship to complete this quest!"));
  body.append("</p><p>");
  body.append(tr("No payment has been registered for this order!"));
  body.append("</p>");
  QMessageBox::warning(this, tr("Invalid relationship"), body);
}

bool OrderEditor::checkOrderCloseStatus() {
  bool out_status = false;
  // Nur Ausführen wenn importSqlResult nicht Leer ist.
  if (sqlQueryResult.size() < 5)
    return out_status;

  // OrdersPaymentBox
  int payment_status = o_payment_status->value().toInt();
  bool payment_status_update = false;

  // OrderStatusBox
  int order_status = o_order_status->value().toInt();
  bool order_status_update = false;

  // Suche nach Änderungen
  QListIterator<DataField> it(sqlQueryResult);
  while (it.hasNext()) {
    DataField df = it.next();
    if (df.field() == o_payment_status->objectName())
      payment_status_update = (payment_status != df.value().toInt());

    if (df.field() == o_order_status->objectName())
      order_status_update = (order_status != df.value().toInt());
  }

  // Abfrage - Update auf Bezahlt?
  if (payment_status_update) { //  && payment_status == 1
    out_status = (out_status != true) ? true : out_status;
  }

  // Abfrage - Update auf Geschlossen?
  if (order_status_update && order_status == ORDER_STATUS_CLOSED) {
    out_status = (out_status != true) ? true : out_status;
  }

  // Abfrage - Update auf Storniert?
  if (order_status_update && order_status == ORDER_STATUS_CANCEL) {
    out_status = (out_status != true) ? true : out_status;
  }

  return out_status;
}

void OrderEditor::restoreOrderStatus() {
  QListIterator<DataField> it(sqlQueryResult);
  while (it.hasNext()) {
    DataField df = it.next();
    if (df.field() == o_order_status->objectName()) {
      o_order_status->setValue(df.value());
      break;
    }
  }
}

bool OrderEditor::updateOrderStatus(int status) {
  if (status < ORDER_STATUS_CLOSED)
    return false;

  qDebug() << Q_FUNC_INFO << status;

  QString body("<p>");
  body.append(tr("Do you really want to close this order and pass it on "
                 "to accounting?"));
  body.append("</p><p>");
  body.append(tr("If so, the entry will no longer be visible here!"));
  body.append("</p>");

  int order_id = o_id->value().toInt();
  if (status == ORDER_STATUS_CANCEL) {
    int ret = QMessageBox::question(this, tr("Cancel order"), body);
    if (ret == QMessageBox::Yes) {
      if (sendSqlQuery(progresUpdate(order_id, status))) {
        emit s_statusMessage(tr("Order deactivated!"));
        return true;
      }
    } else {
      restoreOrderStatus();
      return false;
    }
  }

  if (o_payment_status->value().toInt() == 0) {
    invalidRelationship();
    return false;
  }

  int ret = QMessageBox::question(this, tr("Finish order"), body);
  if (ret == QMessageBox::Yes) {
    if (sendSqlQuery(progresUpdate(order_id, status))) {
      emit s_statusMessage(tr("Order deactivated!"));
      // Statistiken modifizieren!
      int c_id = o_customer_id->value().toInt();
      if (c_id > 0) {
        QString sql = queryUpdateTransaction(c_id);
        if (SHOW_ORDER_SQL_QUERIES) {
          qDebug() << Q_FUNC_INFO << sql << c_id << status;
        }
        m_sql->query(sql);
        if (!m_sql->lastError().isEmpty()) {
          qDebug() << Q_FUNC_INFO << m_sql->lastError();
          return false;
        }
      }
      return true;
    }
  } else {
    restoreOrderStatus();
  }
  return false;
}

bool OrderEditor::sendSqlQuery(const QString &sqlStatement) {
  if (SHOW_ORDER_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sqlStatement;
  }

  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    sqlErrnoMessage(sqlStatement, m_sql->fetchErrors());
    return false;
  }

  if (q.next()) {
    if (!q.record().isNull("o_id")) {
      o_id->setValue(q.value("o_id"));
    }
    if (!q.record().isNull("a_payment_id")) {
      last_payment_id = q.value("a_payment_id").toInt();
    }
  }
  if (showSuccessFully) {
    sqlSuccessMessage(tr("Order saved successfully!"));
  }
  resetModified(inputList);
  m_paymentList->setModified(false);
  emit s_isModified(false);
  return true;
}

bool OrderEditor::createSqlArticleOrder() {
  int payments = m_paymentList->payments();
  if (payments < 1) {
    qWarning("Create SQL Article Order - Missing Payments");
    return false;
  }

  /**
   * @warning Die Id's "oid" und "cid" sind hier zwingend Notwendig!
   */
  int oid = o_id->value().toInt();
  if (oid < 1) {
    qWarning("Missing Order ID");
    return false;
  }

  int cid = o_customer_id->value().toInt();
  if (o_customer_id->value().toString().isEmpty() || cid < 1) {
    qWarning("Missing Customer ID");
    return false;
  }

  QStringList queries;
  for (int row = 0; row < payments; row++) {
    QStringList fields;
    QStringList values;
    QHash<QString, QString> updateSet;
    QHashIterator<QString, QVariant> it(m_paymentList->getTableRow(row));
    int primaryIndex = 0;
    while (it.hasNext()) {
      it.next();
      QString key = it.key();
      if (key == "a_payment_id") {
        int index = it.value().toInt();
        if (index >= 1) {
          primaryIndex = index;
        }
        continue;
      }
      fields.append(key);
      if (key == "a_order_id") {
        updateSet.insert(key, o_id->value().toString());
        values.append(o_id->value().toString());
        continue;
      } else if (key == "a_customer_id") {
        updateSet.insert(key, o_customer_id->value().toString());
        values.append(o_customer_id->value().toString());
        continue;
      }

      QString data = it.value().toString();
      if (it.value().type() == QVariant::String) {
        data = "'" + it.value().toString() + "'";
        updateSet.insert(key, data);
        values.append(data);
      } else {
        updateSet.insert(key, data);
        values.append(data);
      }
    }
    if (primaryIndex > 0) {
      QString sql("UPDATE article_orders SET ");
      QHashIterator<QString, QString> fields(updateSet);
      while (fields.hasNext()) {
        fields.next();
        sql.append(fields.key() + "=" + fields.value() + ",");
      }
      sql.append("a_modified=CURRENT_TIMESTAMP WHERE a_payment_id=");
      sql.append(QString::number(primaryIndex));
      sql.append(";");
      queries.append(sql);
    } else {
      QString sql("INSERT INTO article_orders (");
      sql.append(fields.join(","));
      sql.append(") VALUES (");
      sql.append(values.join(","));
      sql.append(") RETURNING a_payment_id;");
      queries.append(sql);
    }
  }
  if (sendSqlQuery(queries.join(" "))) {
    /**
     * @note Wir müssen hier alles neu einlesen!
     * Weil "a_payment_id" sonst in der ArtikelTabelle
     * fehlt und es zu Doppelten Einträgen kommt!
     */
    const QList<OrderArticle> list = getOrderArticles(oid, cid);
    if (list.size() >= 1) {
      m_paymentList->clearTable();
      m_paymentList->importPayments(list);
      m_paymentList->setModified(false);
    }
    return true;
  }
  return false;
}

void OrderEditor::createSqlUpdate() {
  bool leaveAfterUpdate = false;
  QString oid = o_id->value().toString();
  if (oid.isEmpty())
    return;

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    generateDeliveryNumber();
  }

  o_id->setRequired(true);
  o_invoice_id->setRequired(true);
  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "o_id")
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  // PoPup Speichern unterdrücken
  showSuccessFully = false;

  // Artikel Bestelliste aktualisieren
  if (!createSqlArticleOrder()) {
    qWarning("UPDATE canceld");
    return;
  }

  // Validierung des aktuellen Bestellstatus
  int order_status = o_order_status->value().toInt();
  if (order_status >= ORDER_STATUS_CLOSED) {
    if (checkOrderCloseStatus()) {
      bool status = false;
      if (order_status == ORDER_STATUS_CLOSED) {
        status = updateOrderStatus(order_status);
      } else if (order_status == ORDER_STATUS_CANCEL) {
        status = updateOrderStatus(order_status);
      }
      if (!status) {
        restoreOrderStatus();
        qWarning("Order: Save operation aborted!");
        return;
      }
      leaveAfterUpdate = status;
    }
  }
  // PoPup Speichern wieder anzeigen!
  showSuccessFully = true;

  QString sql("UPDATE inventory_orders SET ");
  sql.append(set.join(","));
  sql.append(",o_modified=CURRENT_TIMESTAMP");
  if (o_id->value().toInt() == ORDER_STATUS_DELIVERED) {
    sql.append(",o_delivered=CURRENT_TIMESTAMP");
  }
  sql.append(" WHERE o_id=");
  sql.append(oid);
  sql.append(";");
  if (sendSqlQuery(sql) && leaveAfterUpdate) {
    int c_id = o_customer_id->value().toInt();
    if (c_id > 0) {
      m_sql->query(queryUpdatePurchases(c_id));
      if (!m_sql->lastError().isEmpty()) {
        qDebug() << Q_FUNC_INFO << m_sql->lastError();
      }
    }
    finalLeaveEditor();
  }
}

void OrderEditor::createSqlInsert() {
  if (!o_id->value().toString().isEmpty())
    return;

  o_id->setRequired(false);
  o_invoice_id->setRequired(false);
  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList fields;
  QStringList values;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "o_id")
      continue;

    if (ignoreOnInsert.contains(it.key(), Qt::CaseSensitive))
      continue;

    fields.append(it.key());
    if (it.value().type() == QVariant::String) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_orders (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(") RETURNING o_id;");

  bool result = sendSqlQuery(sql);
  if (result && !o_id->value().toString().isEmpty()) {
    /**< Erstelle Lieferschein Nummer */
    generateDeliveryNumber();

    initInvoiceNumber(o_id->value().toInt());

    /**
     * Wenn die Artikel-Bestellisten Tabelle nicht leer ist.
     * Dann createSqlArticleOrder() aufrufen!
     */
    m_paymentList->setEnabled(true);
    if (m_paymentList->payments() > 0) {
      createSqlArticleOrder();
    }
    articleCountUpdate(o_id->value().toInt());
  }
}

void OrderEditor::articleCountUpdate(int oid) {
  QString sql = inventoryArticleCounts(oid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      if (q.value("count").toInt() >= 0) {
        emit s_articleCount(q.value("a_article_id").toInt(),
                            q.value("count").toInt());
      }
    }
  } else {
    if (q.lastError().type() != QSqlError::NoError) {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
      sqlErrnoMessage(sql, m_sql->fetchErrors());
    }
  }
}

void OrderEditor::setData(const QString &key, const QVariant &value,
                          bool required) {
  if (key.isEmpty()) {
    qWarning("No setData Key:%s", qPrintable(key));
    return;
  }

  UtilsMain *inp = findChild<UtilsMain *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    if (required && !inp->isRequired())
      inp->setRequired(required);

    return;
  }

  /**
   * Wird von "o_delivery_service" zu verfügung gestellt!
   */
  if (key == "o_delivery_package") {
    o_delivery_service->setDeliveryPackage(value.toInt());
    return;
  }
  qDebug() << "Missing k:" << key << " v:" << value << " r:" << required;
}

void OrderEditor::findCustomer(int cid) {
  if (cid <= 1)
    return;

  if (!getCustomerAddress(cid)) {
    qWarning("No Customer Data");
  }
}

void OrderEditor::findArticle(int aid) {
  QString select = inventoryArticle(aid);
  if (select.isEmpty())
    return;

  if (SHOW_ORDER_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    q.next();
    OrderArticle data;
    data.setPayment(-1); // Darf keine positive Zahl sein!
    data.setArticle(q.value("aid").toInt());
    int count = q.value("counts").toInt();
    if (count > 0) {
      // Wir fügen immer nur ein Artikel ein!
      count = 1;
    }
    data.setCount(count);
    data.setPrice(q.value("price").toDouble());
    data.setSellPrice(q.value("price").toDouble());
    data.setTitle(q.value("title").toString());
    data.setSummary(
        tr("Article %1, Price %2, Count: %3, Title: %4")
            .arg(q.value("aid").toString(), q.value("price").toString(),
                 q.value("counts").toString(), q.value("title").toString()));

    m_paymentList->addArticleRow(data);
  } else {
    qDebug() << m_sql->lastError();
  }
}

void OrderEditor::findRemoveTableRow(int row) {
  QHash<QString, QVariant> items(m_paymentList->getTableRow(row));
  QString pId = items.value("a_payment_id").toString();
  QString aId = items.value("a_article_id").toString();
  QString body("<p>");
  body.append(tr("Do you really want to remove this Article from the list?"));
  body.append("</p><p>" + items.value("a_title").toString() + "</p><p>");
  body.append("</p><p>");
  body.append(tr("If so, the entry will no longer be visible here!"));
  body.append("</p>");
  int ret = QMessageBox::question(this, tr("Delete"), body);
  if (ret == QMessageBox::Yes) {
    m_paymentList->removeTableRow(row);
    showSuccessFully = false;
    if (sendSqlQuery(paymentRemove(pId, aId))) {
      emit s_statusMessage(tr("Item removed!"));
      m_paymentList->setModified(true);
    }
    showSuccessFully = true;
  }
  pId.clear();
  aId.clear();
  items.clear();
}

void OrderEditor::saveData() {
  if (o_id->value().toString().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
  }
}

void OrderEditor::checkLeaveEditor() {
  QString notify(tr("Unsaved Changes, don't leave this page before saved."));
  if (checkIsModified(p_objPattern)) {
    emit s_postMessage(notify);
    return;
  }
  /** @note Muss Manuel gesetzt werden */
  if (m_paymentList->isModified()) {
    emit s_postMessage(notify);
    return;
  }
  finalLeaveEditor();
}

void OrderEditor::clearEditorFields() {
  // SQL History leeren
  sqlQueryResult.clear();
  // Alle Datenfelder leeren
  clearDataFields(p_objPattern);
  o_delivery_add_price->setChecked(false);
  // FIXME Sollte hier nicht stehen!
  o_vat_levels->setCurrentIndex(0);
  o_vat_country->setValue(QLocale::system().bcp47Name());
  // Tabelle leeren
  m_paymentList->clearTable();
}

void OrderEditor::finalLeaveEditor() {
  clearEditorFields();
  emit s_leaveEditor();
}

void OrderEditor::openPrinterDeliveryDialog() {
  int oid = o_id->value().toInt();
  if (oid < 1) {
    emit s_postMessage(tr("Missing Order-Id"));
    return;
  }

  int cid = o_customer_id->value().toInt();
  if (cid < 1) {
    emit s_postMessage(tr("Missing Customer-Id"));
    return;
  }

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    did = deliveryNumber();
    o_delivery->setValue(did);
  }

  DeliveryNote *dialog = new DeliveryNote(this);
  dialog->setDelivery(oid, cid, did);
  // Address
  QString c_add;
  QString sql = queryCustomerDeliveryAddress(cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    c_add.append(q.value(0).toString());
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Customer Address found"));
    return;
  }
  dialog->setCustomerAddress(c_add);
  // Articles
  QList<DeliveryNote::Delivery> deliveries;
  q = m_sql->query(queryDeliveryNotes(oid, cid));
  QRegExp strip("\\-\\s+\\-");
  while (q.next()) {
    DeliveryNote::Delivery d;
    d.articleid = q.value("aid").toString();
    d.designation = q.value("title").toString().replace(strip, "-");
    d.quantity = q.value("quant").toString();
    deliveries.append(d);
  }
  if (deliveries.count() < 1) {
    qDebug() << m_sql->lastError();
    emit s_postMessage(tr("no deliveries found"));
    return;
  }
  // Start Dialog
  if (dialog->exec(deliveries)) {
    /* Unused */
  }
  deliveries.clear();
}

void OrderEditor::openPrinterInvoiceDialog() {
  int oid = o_id->value().toInt();
  if (oid < 1) {
    emit s_postMessage(tr("Missing Order-Id"));
    return;
  }

  int cid = o_customer_id->value().toInt();
  if (cid < 1) {
    emit s_postMessage(tr("Missing Customer-Id"));
    return;
  }

  int in_id = o_invoice_id->value().toInt();
  if (in_id < 1) {
    emit s_postMessage(tr("Missing Invoice-Id"));
    return;
  }

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    did = deliveryNumber();
    o_delivery->setValue(did);
  }

  ApplSettings cfg;
  Invoice *dialog = new Invoice(this);
  dialog->setInvoice(oid, cid, in_id, did);
  // Address
  QString c_add;
  QString sql = queryCustomerInvoiceAddress(cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    c_add.append(q.value("c_postal_address").toString());
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Customer Address found"));
    return;
  }
  dialog->setCustomerAddress(c_add);

  bool paid = (o_payment_status->status() == OrdersPaymentBox::Yes);

  QList<BillingInfo> list = getBillingInfo(oid, cid);

  if (dialog->exec(list, paid) == QDialog::Rejected) {
    emit s_statusMessage(tr("Printing canceled."));
  }
  list.clear();
}

void OrderEditor::openPrinterPaymentReminderDialog() {
  int oid = o_id->value().toInt();
  if (oid < 1) {
    emit s_postMessage(tr("Missing Order-Id"));
    return;
  }

  int cid = o_customer_id->value().toInt();
  if (cid < 1) {
    emit s_postMessage(tr("Missing Customer-Id"));
    return;
  }

  int in_id = o_invoice_id->value().toInt();
  if (in_id < 1) {
    emit s_postMessage(tr("Missing Invoice-Id"));
    return;
  }

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    emit s_postMessage(tr("Missing Delivery-Id"));
    return;
  }

  // Address
  QString c_add;
  QString sql = queryCustomerInvoiceAddress(cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    c_add.append(q.value(0).toString());
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Customer Address found"));
    return;
  }

  QString message;
  QString person;
  sql = queryCustomerPaymentReminder(cid, "PDF_PAYMENT_REMINDER");
  q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    person = q.value("person").toString();
    message = q.value("tb_message").toString();
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Message available"));
    return;
  }

  if (message.isEmpty()) {
    emit s_postMessage(tr("No Message available"));
    return;
  }

  message.replace("@CRM_CUSTOMER_NAME@", person);

  PaymentReminder *dialog = new PaymentReminder(this);
  dialog->setCustomerAddress(c_add);
  dialog->setPaymentInfo(oid, cid, in_id, did);
  dialog->setTitleText(tr("1. Payment Reminder"));
  dialog->setMainText(message);
  dialog->setFinalText(tr("If the amount has been paid in the meantime, please "
                          "send us a short message."));

  QList<BillingInfo> list = getBillingInfo(oid, cid);
  if (dialog->exec(list) == QDialog::Rejected) {
    emit s_statusMessage(tr("Printing canceled."));
  }
}

void OrderEditor::openEMailDialog(const QString &tpl) {
  int cid = o_customer_id->value().toInt();
  if (cid < 1)
    return;

  int oid = o_id->value().toInt();
  if (oid < 1)
    return;

  if(!m_cfg->contains("dirs/mailapplication")) {
    emit s_statusMessage(tr("Missing eMail configuration!"));
    return;
  }

  MailForwardDialog *d = new MailForwardDialog(this);
  d->setOrderId(oid);
  if (d->exec(cid, tpl) == QDialog::Rejected) {
    emit s_statusMessage(tr("Mail canceled."));
  }
}

void OrderEditor::generateDeliveryNumber() {
  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    o_delivery->setValue(deliveryNumber());
  }
}

void OrderEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void OrderEditor::showMessagePoUp(const QString &str) {
  QMessageBox::information(this, tr("Notice"), str, QMessageBox::Ok);
}

void OrderEditor::initDefaults() {
  setInputList();
  // Artikelliste leeren
  m_paymentList->clearTable();
  resetModified(inputList);
  setEnabled(true);
  o_delivery_service->loadSqlDataset();
  if (o_provider_name->value().toString().isEmpty())
    o_provider_name->setValue(tr("Internal"));

  if (o_provider_order_id->value().toString().isEmpty())
    o_provider_order_id->setValue(tr("Internal"));
}

void OrderEditor::initInvoiceNumber(int orderId) {
  QString sql("SELECT o_invoice_id FROM ");
  sql.append("inventory_orders WHERE o_id=");
  sql.append(QString::number(orderId));
  sql.append(";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() >= 1) {
    while (q.next()) {
      o_invoice_id->setValue(q.value("o_invoice_id"));
    }
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

const QList<BillingInfo> OrderEditor::getBillingInfo(int orderId,
                                                     int customerId) {
  /**
   * Wenn es kein Europaland ist, dann fällt auch keine Mehrwertsteuer an!
   * Die Zeile MwST dann in der Rechnung ausblenden!
   */
  bool disable_vat = (o_vat_country->value().toString().isEmpty());

  /**
   * Wenn die Versandkosten mit berechnet werden sollen.
   * Muss putIntoInvoice() true zurück geben und der Paketpreis
   * wird von 0.00 auf den Versandwert angehoben!
   * @note Wenn die Versandkosten nicht '0.00' sind werden sie in
   * der Rechnung aufgeführt!
   */
  int vIndex = o_vat_levels->currentIndex();
  int setTax = o_vat_levels->itemData(vIndex, Qt::UserRole).toInt();
  if (setTax == 0) {
    o_vat_included->setChecked(false);
    disable_vat = true;
  }

  bool vat_included = o_vat_included->isChecked();

  qreal packagePrice = 0;
  if (o_delivery_add_price->isChecked())
    packagePrice = o_delivery_service->getPackagePrice();

  QList<BillingInfo> list;
  QSqlQuery q = m_sql->query(queryBillingInfo(orderId, customerId));
  if (q.size() > 0) {
    QRegExp strip("\\-\\s+\\-");
    while (q.next()) {
      BillingInfo d;
      d.articleid = q.value("aid").toString();
      d.designation = q.value("title").toString().replace(strip, "-");
      d.quantity = q.value("quant").toInt();
      d.sellPrice = q.value("sellPrice").toDouble();
      d.includeVat = vat_included;
      d.disableVat = disable_vat;
      d.taxValue = setTax;
      d.packagePrice = packagePrice;
      list.append(d);
    }
  } else {
    list.clear();
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Billing Info found"));
  }
  return list;
}

const QList<OrderArticle> OrderEditor::getOrderArticles(int oid, int cid) {
  QList<OrderArticle> list;
  QString sql = paymentArticleOrders(oid, cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() >= 1) {
    QSqlRecord r = q.record();
    while (q.next()) {
      OrderArticle order;
      for (int f = 0; f < r.count(); f++) {
        QString fn(r.fieldName(f));
        if (fn == "a_payment_id") {
          order.setPayment(q.value(fn).toInt());
        } else if (fn == "a_order_id") {
          order.setOrder(q.value(fn).toInt());
        } else if (fn == "a_article_id") {
          order.setArticle(q.value(fn).toInt());
        } else if (fn == "a_customer_id") {
          order.setCustomer(q.value(fn).toInt());
        } else if (fn == "a_price") {
          order.setPrice(q.value(fn).toDouble());
        } else if (fn == "a_sell_price") {
          order.setSellPrice(q.value(fn).toDouble());
        } else if (fn == "a_count") {
          order.setCount(q.value(fn).toInt());
        } else if (fn == "a_title") {
          order.setTitle(q.value(fn).toString());
        }
      }
      list.append(order);
    }
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
  return list;
}

bool OrderEditor::getCustomerAddress(int cid) {
  if (cid < 1)
    return false;

  QString select = queryCustomerInfoAddress(cid);
  if (SHOW_ORDER_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  QSqlQuery q = m_sql->query(select);
  QString buffer;
  QString email;
  if (q.size() > 0) {
    while (q.next()) {
      buffer.append(q.value("c_postal_address").toString());
      email = q.value("c_email_0").toString();
    }
    m_customer_address->setText(buffer + "\n");
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }

  if (!email.isEmpty()) {
    m_actionBar->setViewMailButton(true);
  } else {
    qInfo("No Customer eMail Address found.");
  }
  return true;
}

const QString OrderEditor::deliveryNumber() {
  int order_id = o_id->value().toInt();
  QString f;
  QDate date = QDate::currentDate();
  f.append(QString::number(date.year()));
  f.append(QString::number(date.dayOfYear()));
  f.append(QString::number(order_id));
  return f;
}

void OrderEditor::addArticleId(int articleId) {
  m_paymentList->insertSearchId(articleId);
}

bool OrderEditor::openUpdateOrder(int oid) {
  initDefaults();
  if (oid < 1) {
    qWarning("Empty o_id ...");
    return false;
  }
  m_paymentList->setEnabled(true);

  QString select("SELECT * FROM inventory_orders WHERE");
  select.append(" o_id=" + QString::number(oid) + ";");

  if (SHOW_ORDER_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  int cid = -1;
  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    QSqlRecord r = m_sql->record("inventory_orders");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        // qDebug() << Q_FUNC_INFO << key << val << required;
        if (key == "o_customer_id") {
          cid = val.toInt();
        }
        DataField d;
        d.setField(key);
        d.setType(val.type());
        d.setRequired(required);
        d.setValue(val);
        sqlQueryResult.append(d);
      }
    }
  } else {
    if (m_sql->lastError().isEmpty()) {
      return false;
    }
    sqlErrnoMessage(m_sql->fetchErrors(), select);
    return false;
  }

  if (cid >= 1) {
    const QList<OrderArticle> list = getOrderArticles(oid, cid);
    if (list.size() >= 1)
      m_paymentList->importPayments(list);
  }

  if (!sqlQueryResult.isEmpty())
    importSqlResult();

  return true;
}

void OrderEditor::openCreateOrder(int cid) {
  initDefaults();
  if (cid > 0) {
    m_paymentList->setEnabled(false);
    o_customer_id->setValue(cid);
    if (getCustomerAddress(cid)) {
      emit isModified(true);
    }
  }
}

void OrderEditor::openCreateOrder(const ProviderOrder &order) {
  ProviderOrder copy(order);
  int cid = copy.customerId();
  if (cid < 1) {
    emit s_postMessage(tr("can't create order without costumer Id."));
    return;
  }
  // Aufräumen
  if (o_customer_id->value().toInt() > 0 || m_paymentList->payments() > 0) {
    clearEditorFields();
  }

  initDefaults();
  o_customer_id->setValue(cid);
  o_provider_order_id->setValue(copy.providerId());
  o_provider_name->setValue(copy.provider());
  o_delivery_service->setValue(ORDER_DELIVERY_SERVICE);
  if (getCustomerAddress(cid)) {
    QList<OrderArticle> list;
    foreach (QString said, copy.articleIds()) {
      int aid = said.toInt();
      QString sql = inventoryArticle(aid);
      QSqlQuery q = m_sql->query(sql);
      if (q.size() > 0) {
        // QSqlRecord r = q.record();
        while (q.next()) {
          OrderArticle d;
          d.setPayment(-1);
          d.setArticle(q.value("aid").toInt());
          d.setOrder(-1);
          d.setCustomer(cid);
          int count = q.value("counts").toInt();
          if (count > 0) {
            // Wir fügen immer nur ein Artikel ein!
            count = 1;
          }
          d.setCount(count);
          d.setPrice(q.value("price").toDouble());
          d.setSellPrice(q.value("price").toDouble());
          d.setTitle(q.value("title").toString());
          d.setSummary(tr("Article %1, Price %2, Count: %3, Title: %4")
                           .arg(q.value("aid").toString(),
                                q.value("price").toString(),
                                q.value("counts").toString(),
                                q.value("title").toString()));

          list.append(d);
        }
      } else {
        QString sqlError = m_sql->lastError();
        if (sqlError.isEmpty()) {
          QString info("<p>");
          info.append(tr("One or more items on the list are not available!"));
          info.append("</p><p>");
          info.append(tr("Therefore, they cannot be added"));
          info.append("</p>");
          QMessageBox::warning(this, tr("Order"), info, QMessageBox::Ok);
        } else {
          sqlErrnoMessage(sqlError, sql);
        }
        m_paymentList->setEnabled(false);
        return;
      }
    }
    m_paymentList->importPayments(list);
    m_paymentList->setEnabled(false);
    emit isModified(true);
  }
}
