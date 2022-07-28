// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interface.h"
#include "purchasedebugtable.h"
#include "purchasepaymentinfo.h"
#include "purchaserordertable.h"

#include <QFontMetricsF>
#include <QGridLayout>
#include <QIcon>
#include <QVBoxLayout>

namespace Antiqua {

PurchaseOverview::PurchaseOverview(const QString &id, QWidget *parent)
    : QWidget{parent} {
  setObjectName("purchase_overview_" + id);

  QIcon qi1 = style()->standardIcon(QStyle::SP_MessageBoxQuestion);
  QIcon qi2 = style()->standardIcon(QStyle::SP_FileIcon);
  QFontMetricsF fm(font());
  qreal mWidth = fm.horizontalAdvance("999999999");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN Artikelanzeige
  m_table = new Antiqua::PurchaserOrderTable(this);
  layout->addWidget(m_table);
  // END

  // Begin ToolBar
  m_toolbar = new QToolBar(this);
  m_toolbar->setMovable(false);
  m_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_toolbar->addWidget(new QLabel(tr("Customer") + ": "));
  m_customerId = new QLineEdit(m_toolbar);
  m_customerId->setReadOnly(true);
  m_customerId->setMaxLength(20);
  m_customerId->setMaximumWidth(mWidth);
  m_toolbar->addWidget(m_customerId);
  m_toolbar->addSeparator();
  QString str_customer_info = tr("create") + " " + tr("Purchaser");
  btn_createCustomer = new QPushButton(qi1, str_customer_info, m_toolbar);
  btn_createCustomer->setToolTip(tr("if customer not exists, create it."));
  btn_createCustomer->setEnabled(true);
  m_toolbar->addWidget(btn_createCustomer);
  m_toolbar->addSeparator();
  m_customerInfo = new QLineEdit(m_toolbar);
  m_customerInfo->setObjectName("person");
  m_customerInfo->setReadOnly(true);
  m_toolbar->addWidget(m_customerInfo);
  m_toolbar->addSeparator();
  QString str_article_info = tr("Orders") + " ";
  m_toolbar->addWidget(new QLabel(str_article_info, m_toolbar));
  btn_checkArticle = new QPushButton(qi1, tr("execute check"), m_toolbar);
  btn_checkArticle->setToolTip(
      tr("Create a search query to see if all items are available."));
  m_toolbar->addWidget(btn_checkArticle);
  layout->addWidget(m_toolbar);
  // END

  // BEGIN Customer purchase info
  QWidget *m_overview = new QWidget(this);
  QVBoxLayout *viewLayout = new QVBoxLayout(m_overview);
  viewLayout->setContentsMargins(0, 0, 0, 0);

  m_tabWidget = new QTabWidget(m_overview);
  viewLayout->addWidget(m_tabWidget);
  // Addresses
  int tabIndex = 0;
  QWidget *addressTab = new QWidget(m_tabWidget);
  QGridLayout *gridLayout = new QGridLayout(addressTab);
  QLabel *bainfo = new QLabel(tr("billing address"), addressTab);
  gridLayout->addWidget(bainfo, 0, 0, 1, 1);
  QLabel *sainfo = new QLabel(tr("shipping address"), addressTab);
  gridLayout->addWidget(sainfo, 0, 1, 1, 1);
  m_billingAddress = new QTextEdit(addressTab);
  m_billingAddress->setObjectName("c_postal_address");
  gridLayout->addWidget(m_billingAddress, 1, 0, 1, 1);
  m_shippingAddress = new QTextEdit(addressTab);
  m_shippingAddress->setObjectName("c_shipping_address");
  gridLayout->addWidget(m_shippingAddress, 1, 1, 1, 1);
  m_tabWidget->insertTab(tabIndex++, addressTab, qi2, tr("addresses"));
  // Zahlungs Informationen
  m_paymentInfo = new Antiqua::PurchasePaymentInfo(this);
  m_paymentInfo->setObjectName("m_paymentInfo");
  m_tabWidget->insertTab(tabIndex++, m_paymentInfo, qi1,
                         tr("Payment Information"));

  // Kunden Kommentar
  m_comments = new QTextEdit(m_tabWidget);
  m_comments->setObjectName("o_delivery_comment");
  m_tabWidget->insertTab(tabIndex++, m_comments, qi1, tr("customer comment"));

#ifdef ANTIQUA_DEVELOPEMENT
  // Informationen
  m_debugTable = new PurchaseDebugTable(m_tabWidget);
  m_tabWidget->insertTab(tabIndex++, m_debugTable, qi1, tr("Developement"));
#endif

  m_tabWidget->setCurrentIndex(0);
  m_overview->setLayout(viewLayout);
  layout->addWidget(m_overview);
  // END

  layout->addStretch(1);
  setLayout(layout);

  // Weiterleitung Artikel mit Nummer öffnen.
  connect(m_table, SIGNAL(inspectArticle(int)), this,
          SIGNAL(inspectArticle(int)));

  // Weiterleitung Artikel Nummern prüfen
  connect(m_table, SIGNAL(findArticleNumbers()), this, SIGNAL(checkOrders()));
  connect(btn_checkArticle, SIGNAL(clicked()), this, SIGNAL(checkOrders()));

  // Kunde prüfen/anlegen
  connect(m_customerId, SIGNAL(textChanged(const QString &)), this,
          SLOT(customerChanged(const QString &)));
  connect(btn_createCustomer, SIGNAL(clicked()), this, SIGNAL(checkCustomer()));
}

void PurchaseOverview::customerChanged(const QString &id) {
  if (id.trimmed().length() < 1)
    return;

  int cid = getCustomerId();
  if (cid > 0) {
    btn_createCustomer->setEnabled(false);
    emit customerIdChanged(cid);
  }
}

void PurchaseOverview::setCustomerId(int id) {
  m_customerId->setText(QString::number(id));
}

int PurchaseOverview::getCustomerId() {
  bool b = false;
  QString num = m_customerId->text();
  int id = num.toInt(&b);
  if (!b || id < 1)
    return 0;

  return id;
}

const QStringList PurchaseOverview::customerSearchFields() const {
  QStringList l({"c_firstname", "c_lastname", "c_postalcode", "c_location"});
  return l;
}

const QList<int> PurchaseOverview::getArticleIDs() {
  QList<int> l;
  for (int r = 0; r < m_table->rowCount(); r++) {
    QTableWidgetItem *item = m_table->item(r, 1);
    if (item != nullptr) {
      l.append(item->data(Qt::DisplayRole).toInt());
    }
  }
  return l;
}

void PurchaseOverview::setTableCount(int count) { m_table->setRowCount(count); }

int PurchaseOverview::getTableCount() { return m_table->rowCount(); }

bool PurchaseOverview::setTableData(int row, int column,
                                    const QVariant &value) {
  QString title = value.toString();
  QTableWidgetItem *item = m_table->createItem(title);
  m_table->setItem(row, column, item);
  return true;
}

const QVariant PurchaseOverview::getTableData(int row, int column) {
  QTableWidgetItem *item = m_table->item(row, column);
  if (item != nullptr)
    return item->data(Qt::DisplayRole);

  return QVariant();
}

void PurchaseOverview::setValue(const QString &objName, const QVariant &value) {
  if (objName.isEmpty() || value.isNull())
    return;

  if (objName.contains("_phone_") || objName.contains("_mobil_")) {
    setPhone(objName, value);
    return;
  }

  QLineEdit *le = findChild<QLineEdit *>(objName, Qt::FindChildrenRecursively);
  if (le != nullptr) {
    le->setText(value.toString().trimmed());
    return;
  }

  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    tx->setPlainText(value.toString().trimmed());
    return;
  }

  // Zahlungs Informationen
  m_paymentInfo->setData(objName, value);

#ifdef ANTIQUA_DEVELOPEMENT
  int row = m_debugTable->rowCount();
  m_debugTable->setRowCount(row + 1);
  m_debugTable->setItem(row, 0, m_debugTable->createItem(objName));
  m_debugTable->setItem(row, 1, m_debugTable->createItem(value));
#else
  m_purchaseData.insert(objName, value);
#endif
}

void PurchaseOverview::setPhone(const QString &objName, const QVariant &value) {
  QRegExp pattern("[^\\d]+");
  QString phone(value.toString().trimmed());
  phone.replace(pattern, "");
  phone = phone.trimmed();
  if (phone.isEmpty() || phone.length() < 8) {
    qDebug() << "PhoneNumber rejected:" << phone << " FROM:" << value;
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  int row = m_debugTable->rowCount();
  m_debugTable->setRowCount(row + 1);
  m_debugTable->setItem(row, 0, m_debugTable->createItem(objName));
  m_debugTable->setItem(row, 1, m_debugTable->createItem(phone));
#else
  m_purchaseData.insert(objName, phone);
#endif
}

const QVariant PurchaseOverview::getValue(const QString &objName) {
  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    return tx->toPlainText().trimmed();
  }

#ifdef ANTIQUA_DEVELOPEMENT
  for (int r = 0; r < m_debugTable->rowCount(); r++) {
    QTableWidgetItem *item = m_debugTable->item(r, 0);
    if (item->text() == objName) {
      return m_debugTable->item(r, 1)->text();
    }
  }
#else
  QMapIterator<QString, QVariant> it(m_purchaseData);
  while (it.hasNext()) {
    it.next();
    if (it.key() == objName)
      return it.value();
  }
#endif
  return QVariant();
}

const QHash<QString, QVariant> PurchaseOverview::getCustomerData() {
  QHash<QString, QVariant> list;
  int c_id = getCustomerId();
  if (c_id > 0)
    list.insert("c_id", c_id);

#ifdef ANTIQUA_DEVELOPEMENT
  for (int r = 0; r < m_debugTable->rowCount(); r++) {
    QString n = m_debugTable->item(r, 0)->text();
    QString v = m_debugTable->item(r, 1)->text();
    if (n.startsWith("c_", Qt::CaseSensitive))
      list.insert(n, v);
  }
#else
  QMapIterator<QString, QVariant> it(m_purchaseData);
  while (it.hasNext()) {
    it.next();
    if (it.key().startsWith("c_", Qt::CaseSensitive))
      list.insert(it.key(), it.value());
  }
#endif

  QStringList objList({"c_postal_address", "c_shipping_address"});
  foreach (QString n, objList) {
    QTextEdit *te = findChild<QTextEdit *>(n, Qt::FindChildrenRecursively);
    if (te != nullptr) {
      QString data = te->toPlainText().trimmed();
      if (!data.isEmpty())
        list.insert(n, te->toPlainText());
    }
  }
  return list;
}

InterfaceWidget::InterfaceWidget(const QString &widgetId, QWidget *parent)
    : QScrollArea{parent}, p_widgetId(widgetId) {
  setWidgetResizable(true);
  setObjectName(widgetId);
  setWindowTitle(widgetId);

  m_order = new Antiqua::PurchaseOverview(widgetId, this);
  setWidget(m_order);

  // Weiterleitung Artikel öffnen
  connect(m_order, SIGNAL(inspectArticle(int)), this, SIGNAL(openArticle(int)));
  // Weiterleitung Artikelnummern prüfen
  connect(m_order, SIGNAL(checkOrders()), this, SLOT(readCurrentArticleIds()));
  connect(m_order, SIGNAL(checkCustomer()), this, SLOT(checkCustomerClicked()));
}

void InterfaceWidget::setProviderName(const QString &name) {
  providerName = name;
  emit providerChanged();
}

const QString InterfaceWidget::getProviderName() { return providerName; }

void InterfaceWidget::setOrderId(const QString &id) {
  orderId = id;
  emit providerChanged();
}

const QString InterfaceWidget::getOrderId() { return orderId; }

void InterfaceWidget::setOrderExists(int oId) {
  if (oId > 0) {
    orderExists = oId;
    emit orderExistsChanged();
  }
}

int InterfaceWidget::getOrderExists() { return orderExists; }

const QString InterfaceWidget::sqlParam(const QString &attribute) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.key() == attribute)
      return fi.value();
  }
  return QString();
}

const QString InterfaceWidget::apiParam(const QString &value) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.value() == value)
      return fi.key();
  }
  return QString();
}

const QString InterfaceWidget::stripString(const QVariant &val) const {
  QString buf(val.toString());
  buf.replace("'", "`");
  buf = buf.trimmed();
  if (buf == tr("Please select") || buf == "Please select") {
    qInfo("Discarded invalid input");
    return QString();
  }
  return buf;
}

const QJsonValue InterfaceWidget::getString(const QString &objName) {
  QString data = m_order->getValue(objName).toString();
  return QJsonValue(stripString(data));
}

const QJsonValue InterfaceWidget::getNumeric(const QString &objName) {
  int data = m_order->getValue(objName).toInt();
  return QJsonValue(data);
}

const QJsonValue InterfaceWidget::getPrice(const QString &objName) {
  double data = m_order->getValue(objName).toDouble();
  return QJsonValue(data);
}

const ProviderOrder InterfaceWidget::getProviderOrder() {
  ProviderOrder order;
  order.setProvider(providerName);
  order.setProviderId(orderId);
  int cid = m_order->getCustomerId();
  // qDebug() << Q_FUNC_INFO << providerName << orderId << cid;
  if (cid < 1) {
    order.setCustomerId(-1);
    qWarning("Missing Customer Id");
    return order;
  }
  order.setCustomerId(cid);
  int col = 1; /**< ArticleId Cell */
  QStringList ids;
  for (int r = 0; r < m_order->getTableCount(); r++) {
    QString aid = m_order->getTableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}

}; // namespace Antiqua
