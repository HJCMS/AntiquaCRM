// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interface.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QGridLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QStyle>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>

namespace Antiqua {

PurchaserOrderTable::PurchaserOrderTable(QWidget *parent)
    : QTableWidget{parent} {
  setColumnCount(5);
  addHeaderItem(0, tr("Provider"));
  addHeaderItem(1, tr("Article"));
  addHeaderItem(2, tr("Count"));
  addHeaderItem(3, tr("Price"));
  addHeaderItem(4, tr("Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

void PurchaserOrderTable::contextMenuEvent(QContextMenuEvent *e) {
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_remove = m->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                                    tr("inspect article"));
  ac_remove->setObjectName("ac_context_search_article");
  connect(ac_remove, SIGNAL(triggered()), this, SIGNAL(findArticleNumbers()));
  QAction *ac_copy = m->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                                  tr("copy article id"));
  ac_copy->setObjectName("ac_context_ac_copy_article");
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyIdToClipboard()));

  m->exec(e->globalPos());
  delete m;
}

void PurchaserOrderTable::copyIdToClipboard() {
  QString buf = item(currentItem()->row(), 1)->text();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void PurchaserOrderTable::addHeaderItem(int i, const QString &name) {
  QTableWidgetItem *item = new QTableWidgetItem(name, QTableWidgetItem::Type);
  item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  setHorizontalHeaderItem(i, item);
}

QTableWidgetItem *PurchaserOrderTable::createItem(const QString &title) const {
  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setFlags(Qt::ItemIsEnabled);
  return item;
}

ProviderWidget::ProviderWidget(const QString &widgetId, QWidget *parent)
    : QWidget{parent} {}

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

  m_toolbar->addWidget(new QLabel(tr("Customer ID") + ":"));

  m_customerId = new QLineEdit(m_toolbar);
  m_customerId->setReadOnly(true);
  m_customerId->setMaxLength(20);
  m_customerId->setMaximumWidth(mWidth);
  m_toolbar->addWidget(m_customerId);

  m_toolbar->addSeparator();
  m_toolbar->addWidget(new QLabel(tr("Purchaser") + ":"));

  m_customerInfo = new QLineEdit(m_toolbar);
  m_customerInfo->setObjectName("person");
  m_customerInfo->setReadOnly(true);
  m_toolbar->addWidget(m_customerInfo);

  m_toolbar->addSeparator();

  ac_check = m_toolbar->addAction(qi1, tr("check articles"));
  layout->addWidget(m_toolbar);
  // END

  // BEGIN Customer purchase info
  QWidget *m_overview = new QWidget(this);
  QVBoxLayout *viewLayout = new QVBoxLayout(m_overview);
  viewLayout->setContentsMargins(0, 0, 0, 0);

  m_tabWidget = new QTabWidget(m_overview);
  viewLayout->addWidget(m_tabWidget);
  // Addresses
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
  m_tabWidget->insertTab(0, addressTab, qi2, tr("addresses"));
  // Kommentare
  m_comments = new QTextEdit(m_tabWidget);
  m_tabWidget->insertTab(1, m_comments, qi1, tr("comments"));
  // Informationen
  m_summary = new QListWidget(m_tabWidget);
  m_summary->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_summary->setTabKeyNavigation(false);
  m_tabWidget->insertTab(2, m_summary, qi1, tr("summary"));

  m_tabWidget->setCurrentIndex(0);
  m_overview->setLayout(viewLayout);
  layout->addWidget(m_overview);
  // END

  layout->addStretch(1);
  setLayout(layout);

  connect(m_table, SIGNAL(findArticleNumbers()), this, SIGNAL(checkOrders()));
  connect(ac_check, SIGNAL(triggered()), this, SIGNAL(checkOrders()));
}

void PurchaseOverview::setCustomerId(int id) {
  QString num = QString::number(id);
  m_customerId->setText(num);
  emit customerIdChanged(id);
}

int PurchaseOverview::getCustomerId() {
  bool b = false;
  QString num = m_customerId->text();
  int id = num.toInt(&b);
  return (b) ? id : 0;
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
  QTableWidgetItem *item = m_table->createItem(value.toString());
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

  // qDebug() << Q_FUNC_INFO << value << objName;
  QListWidgetItem *item = new QListWidgetItem(m_summary);
  item->setData(Qt::DisplayRole, value);
  item->setData(Qt::UserRole, objName);
  m_summary->addItem(item);
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

  QListWidgetItem *item = new QListWidgetItem(m_summary);
  item->setData(Qt::DisplayRole, phone);
  item->setData(Qt::UserRole, objName);
  m_summary->addItem(item);
}

const QVariant PurchaseOverview::getValue(const QString &objName) {
  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    return tx->toPlainText().trimmed();
  }

  for (int r = 0; r < m_summary->count(); r++) {
    if (m_summary->item(r)->data(Qt::UserRole).toString() == objName)
      return m_summary->item(r)->data(Qt::DisplayRole);
  }

  return QVariant();
}

const QHash<QString, QVariant> PurchaseOverview::getCustomerData() {
  QHash<QString, QVariant> list;
  int c_id = getCustomerId();
  if (c_id > 0)
    list.insert("c_id", c_id);

  for (int r = 0; r < m_summary->count(); r++) {
    QString n = m_summary->item(r)->data(Qt::UserRole).toString();
    QVariant v = m_summary->item(r)->data(Qt::DisplayRole);
    if (n.startsWith("c_", Qt::CaseSensitive))
      list.insert(n, v);
  }

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
    : QScrollArea{parent} {}

const QString InterfaceWidget::sqlParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.key() == key)
      return fi.value();
  }
  return QString();
}

const QString InterfaceWidget::apiParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.value() == key)
      return fi.key();
  }
  return QString();
}

const QString InterfaceWidget::stripString(const QVariant &val) const {
  QString buf(val.toString());
  buf.replace("'", "`");
  buf = buf.trimmed();
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

}; // namespace Antiqua
