#include "orderstatusbox.h"

#include <AntiquaCRM>

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>

OrderStatusBox::OrderStatusBox(QWidget *parent) : UtilsMain{parent} {
  setObjectName("o_order_status");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  m_box = new QComboBox(this);
  m_box->setEditable(false);
  m_box->setDuplicatesEnabled(false);
  m_box->setInsertPolicy(QComboBox::NoInsert);
  OrderStatusList list;
  for (int i = 0; i < list.size(); i++) {
    OrderStatus d = list.status(i);
    m_box->insertItem(d.index(), d.title());
  }
  m_box->setMaxCount(list.size());
  m_box->setCurrentIndex(0);
  layout->addWidget(m_box);

  setModified(false);
  setRequired(false);

  setLayout(layout);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(indexChanged(int)));
}

void OrderStatusBox::indexChanged(int i) {
  p_value = i;
  setValue(i);
  setModified(true);
  emit currentIndexChanged(i);
}

void OrderStatusBox::setValue(const QVariant &val) {
  p_value = val.toInt();
  m_box->setCurrentIndex(p_value);
}

void OrderStatusBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void OrderStatusBox::setFocus() { m_box->setFocus(); }

int OrderStatusBox::findIndex(const QString &search) {
  return m_box->findText(search, Qt::MatchExactly);
}

const QVariant OrderStatusBox::value() { return p_value; }

bool OrderStatusBox::isValid() { return true; }

void OrderStatusBox::setInfo(const QString &info) {
  m_info->setText(info);
  m_box->setToolTip(info);
}

const QString OrderStatusBox::info() { return m_box->toolTip(); }

const QString OrderStatusBox::notes() { return tr("Order status is required"); }

StatusDialog::StatusDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("status_dialog_box");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Edit Order Status"));
  setMinimumWidth(300);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *info = new QLabel(this);
  info->setText(tr("Update current Order status."));
  layout->addWidget(info);

  m_statusBox = new OrderStatusBox(this);
  layout->addWidget(m_statusBox);

  QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
  layout->addWidget(btn);

  setLayout(layout);

  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
}

StatusDialog::StatusDialog(qint64 index, QWidget *parent)
    : StatusDialog{parent} {
  if (index > 0)
    m_statusBox->setValue(index);
}

StatusDialog::StatusDialog(const QString &current, QWidget *parent)
    : StatusDialog{parent} {
  int i = m_statusBox->findIndex(current);
  if (i > 0)
    m_statusBox->setValue(i);
}

int StatusDialog::index() { return m_statusBox->value().toInt(); }
