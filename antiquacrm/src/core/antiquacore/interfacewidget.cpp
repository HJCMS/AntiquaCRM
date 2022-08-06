// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interfacewidget.h"
#include "purchasedebugtable.h"
#include "purchasepaymentinfo.h"
#include "purchaserordertable.h"

#include <QFontMetricsF>
#include <QGridLayout>
#include <QIcon>
#include <QSpacerItem>
#include <QStyle>
#include <QVBoxLayout>

namespace Antiqua {

InterfaceWidget::InterfaceWidget(const QString &orderId, QWidget *parent)
    : QScrollArea{parent}, p_widgetId(orderId) {
  setWidgetResizable(true);
  setObjectName(orderId);
  setWindowTitle(orderId);

  m_purchaseData.clear();

  QWidget *m_centralWidget = new QWidget(this);
  m_centralWidget->setObjectName("purchase_overview_" + orderId);

  QIcon qi1 = style()->standardIcon(QStyle::SP_MessageBoxQuestion);
  QIcon qi2 = style()->standardIcon(QStyle::SP_FileIcon);
  QFontMetricsF fm(font());
  qreal mWidth = fm.horizontalAdvance("999999999");

  QVBoxLayout *layout = new QVBoxLayout(m_centralWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN Artikelanzeige
  m_table = new Antiqua::PurchaserOrderTable(m_centralWidget);
  layout->addWidget(m_table);
  // END

  // Begin ToolBar
  m_toolbar = new QToolBar(m_centralWidget);
  m_toolbar->setMovable(false);
  m_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_toolbar->addWidget(new QLabel(tr("Customer Id") + ": "));
  m_customerId = new QLineEdit(m_toolbar);
  m_customerId->setReadOnly(true);
  m_customerId->setMaxLength(20);
  m_customerId->setMaximumWidth(mWidth);
  m_toolbar->addWidget(m_customerId);
  m_toolbar->addSeparator();
  m_customerInfo = new QLabel(m_toolbar);
  m_customerInfo->setObjectName("person");
  m_customerInfo->setToolTip(tr("Purchaser"));
  m_customerInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  m_toolbar->addWidget(m_customerInfo);
  QString btnText = tr("Check item inventory");
  btn_checkArticle = new QPushButton(qi1, btnText, m_toolbar);
  btn_checkArticle->setToolTip(
      tr("Create a search query to see if all items are available."));
  m_toolbar->addWidget(btn_checkArticle);
  layout->addWidget(m_toolbar);
  // END

  // BEGIN Customer purchase info
  QWidget *m_overview = new QWidget(m_centralWidget);
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
  m_paymentInfo = new PurchasePaymentInfo(this);
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

  // BEGIN Operationen
  m_operationBar = new QFrame(m_centralWidget);
  m_operationBar->setObjectName("operations_tool_bar");
  QHBoxLayout *operationsLayout = new QHBoxLayout(m_operationBar);
  operationsLayout->addStretch(1);
  QLabel *m_infoPrAc = new QLabel(m_operationBar);
  m_infoPrAc->setText(
      tr("Service provider dialog for editing, confirming orders and eMail") +
      ":");
  operationsLayout->addWidget(m_infoPrAc);
  btn_providerActions = new QPushButton(m_operationBar);
  btn_providerActions->setIcon(qi1);
  btn_providerActions->setText(tr("Open"));
  btn_providerActions->setToolTip(tr("Opens the service provider dialog for "
                                     "editing and confirming an order."));
  operationsLayout->addWidget(btn_providerActions);
  m_operationBar->setLayout(operationsLayout);
  layout->addWidget(m_operationBar);
  // END

  layout->addStretch(1);
  m_centralWidget->setLayout(layout);
  setWidget(m_centralWidget);

  // Weiterleitung Artikel öffnen
  connect(m_table, SIGNAL(inspectArticle(int)), this,
          SIGNAL(sendOpenArticle(int)));

  // Artikelnummern prüfen
  connect(m_table, SIGNAL(findArticleNumbers()), this,
          SLOT(readCurrentArticleIds()));
  connect(btn_checkArticle, SIGNAL(clicked()), this,
          SLOT(readCurrentArticleIds()));
  connect(btn_providerActions, SIGNAL(clicked()), this,
          SLOT(createProviderOrderUpdate()));
}

void InterfaceWidget::setProviderName(const QString &name) {
  if (name.length() < 1)
    return;

  providerName = name;
  emit sendProviderChanged(providerName);
}

const QString InterfaceWidget::getProviderName() { return providerName; }

void InterfaceWidget::setOrderId(const QString &oId) {
  if (oId.length() < 1)
    return;

  orderId = oId;
  emit sendOrderIdChanged(orderId);
}

const QString InterfaceWidget::getOrderId() { return orderId; }

void InterfaceWidget::setOrderExists(int oId) {
  if (oId < 1)
    return;

  orderExists = oId;
  emit sendOrderExistsChanged(orderExists);
}

int InterfaceWidget::getOrderExists() { return orderExists; }

void InterfaceWidget::setCustomerId(int cId) {
  if (cId < 1)
    return;

  customerId = cId;
  m_customerId->setText(QString::number(cId));
  emit sendCustomerIdChanged(customerId);
}

int InterfaceWidget::getCustomerId() {
  bool b = false;
  QString num = m_customerId->text().trimmed();
  int cId = num.toInt(&b);
  if (!b) {
    qWarning("can not convert customerId:(%s)", qPrintable(num));
    return customerId;
  }

  if (cId > 0 && cId == customerId) {
    return customerId;
  } else {
    qDebug() << Q_FUNC_INFO << "WARNING: customerId Mismatch";
    return customerId;
  }
}

const QList<int> InterfaceWidget::getArticleIDs() {
  QList<int> l;
  for (int r = 0; r < m_table->rowCount(); r++) {
    QTableWidgetItem *item = m_table->item(r, 1);
    if (item != nullptr) {
      l.append(item->data(Qt::DisplayRole).toInt());
    }
  }
  return l;
}

void InterfaceWidget::setTableCount(int count) { m_table->setRowCount(count); }

int InterfaceWidget::getTableCount() { return m_table->rowCount(); }

bool InterfaceWidget::setTableData(int row, int column, const QVariant &value) {
  QString title = value.toString();
  QTableWidgetItem *item = m_table->createItem(title);
  m_table->setItem(row, column, item);
  return true;
}

const QVariant InterfaceWidget::getTableData(int row, int column) {
  QTableWidgetItem *item = m_table->item(row, column);
  if (item != nullptr)
    return item->data(Qt::DisplayRole);

  return QVariant();
}

void InterfaceWidget::setValue(const QString &objName, const QVariant &value) {
  if (objName.isEmpty() || value.isNull())
    return;

  if (objName.contains("_phone_") || objName.contains("_mobil_")) {
    setPhone(objName, value);
    return;
  }

  if (objName == "person") {
    m_customerInfo->setText(value.toString());
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
  m_purchaseData.insert(objName, value);

#ifdef ANTIQUA_DEVELOPEMENT
  int row = m_debugTable->rowCount();
  m_debugTable->setRowCount(row + 1);
  m_debugTable->setItem(row, 0, m_debugTable->createItem(objName));
  m_debugTable->setItem(row, 1, m_debugTable->createItem(value));
#endif
}

const QVariant InterfaceWidget::getValue(const QString &objName) {
  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    return tx->toPlainText().trimmed();
  }

  QMapIterator<QString, QVariant> it(m_purchaseData);
  while (it.hasNext()) {
    it.next();
    if (it.key() == objName)
      return it.value();
  }

  return QVariant();
}

void InterfaceWidget::setPhone(const QString &objName, const QVariant &value) {
  QRegExp pattern("[^\\d]+");
  QString phone(value.toString().trimmed());
  phone.replace(pattern, "");
  phone = phone.trimmed();
  if (phone.isEmpty() || phone.length() < 8) {
    qDebug() << "PhoneNumber rejected:" << phone << " FROM:" << value;
    return;
  }
  m_purchaseData.insert(objName, phone);

#ifdef ANTIQUA_DEVELOPEMENT
  int row = m_debugTable->rowCount();
  m_debugTable->setRowCount(row + 1);
  m_debugTable->setItem(row, 0, m_debugTable->createItem(objName));
  m_debugTable->setItem(row, 1, m_debugTable->createItem(phone));
#endif
}

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
  return (buf.length() < 1) ? QString() : buf;
}

const QJsonValue InterfaceWidget::getString(const QString &objName) {
  QString data = getValue(objName).toString();
  return QJsonValue(stripString(data));
}

const QJsonValue InterfaceWidget::getNumeric(const QString &objName) {
  int data = getValue(objName).toInt();
  return QJsonValue(data);
}

const QJsonValue InterfaceWidget::getPrice(const QString &objName) {
  double data = getValue(objName).toDouble();
  return QJsonValue(data);
}

const ProviderOrder InterfaceWidget::getProviderOrder() {
  ProviderOrder order;
  order.setProvider(providerName);
  order.setProviderId(orderId);
  int cid = getCustomerId();
  // qDebug() << Q_FUNC_INFO << providerName << orderId << cid;
  if (cid < 1) {
    order.setCustomerId(-1);
    qWarning("Missing Customer Id");
    return order;
  }
  order.setCustomerId(cid);
  int col = 1; /**< ArticleId Cell */
  QStringList ids;
  for (int r = 0; r < getTableCount(); r++) {
    QString aid = getTableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}

}; // namespace Antiqua
