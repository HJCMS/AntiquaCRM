// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerselectcustomer.h"

#include <QDebug>
#include <QLabel>
#include <QListIterator>
#include <QSize>
#include <QVBoxLayout>

ProviderSelectCustomer::ProviderSelectCustomer(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(QSize(600, 300));
  setWindowTitle(tr("Customer selection"));

  QString uniqFields("c_firstname,c_lastname,c_postalcode,c_location");
  p_fields << "c_id";
  p_fields << "c_company_name";
  p_fields << uniqFields.split(",");

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QString info = tr("Several customers were found in the address database that "
                    "could apply to this order.<br/>Please select a customer.");

  QLabel *lbInfo = new QLabel(this);
  lbInfo->setTextFormat(Qt::RichText);
  lbInfo->setWordWrap(true);
  lbInfo->setText(info);
  layout->addWidget(lbInfo);

  m_view = new QTextEdit();
  layout->addWidget(m_view);

  m_list = new QListWidget();
  layout->addWidget(m_list);

  m_btnBox = new QDialogButtonBox();
  m_btnBox->setStandardButtons(QDialogButtonBox::Close | QDialogButtonBox::Ok);
  layout->addWidget(m_btnBox);

  setLayout(layout);

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_list, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(itemSelected(QListWidgetItem *)));
}

bool ProviderSelectCustomer::sqlQueryCustomers(const QString &query) {
  QSqlQuery q = m_sql->query(query);
  if (q.size() > 0) {
    QString buf;
    while (q.next()) {
      int id = q.value("c_id").toInt();
      addListItem(id, q.value("info").toString());
    }
    buf.clear();
    return true;
  } else {
    qDebug() << m_sql->lastError() << query;
  }
  return false;
}

void ProviderSelectCustomer::addListItem(int cid, const QString &str) {
  QListWidgetItem *item = new QListWidgetItem(m_list);
  item->setData(Qt::UserRole, cid);
  item->setData(Qt::DisplayRole, str);
  m_list->addItem(item);
}

void ProviderSelectCustomer::itemSelected(QListWidgetItem *item) {
  int id = item->data(Qt::UserRole).toInt();
  QString sql("SELECT c_id, c_postal_address FROM customers ");
  sql.append("WHERE c_id=" + QString::number(id) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    selected_id = q.value("c_id").toInt();
    m_view->setPlainText(q.value("c_postal_address").toString());
  }
}

int ProviderSelectCustomer::getSelectedCustomer() {
  if (selected_id < 1 && m_list->currentItem() != nullptr)
    return m_list->currentItem()->data(Qt::UserRole).toInt();

  return selected_id;
}

int ProviderSelectCustomer::exec(const QList<int> &cids) {
  QString sql("SELECT c_id, CONCAT_WS(' '," + p_fields.join(","));
  sql.append(") AS info FROM customers WHERE c_id IN ");
  QStringList set;
  QListIterator<int> it(cids);
  while (it.hasNext()) {
    set << QString::number(it.next());
  }
  sql.append("(" + set.join(",") + ");");
  if (!sqlQueryCustomers(sql)) {
    return QDialog::Rejected;
  }
  return QDialog::exec();
}
