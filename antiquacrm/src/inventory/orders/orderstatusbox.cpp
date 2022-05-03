#include "orderstatusbox.h"

#include <AntiquaCRM>

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

OrderStatusBox::OrderStatusBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("o_order_status");
  setEditable(false);
  setDuplicatesEnabled(false);
  setInsertPolicy(QComboBox::NoInsert);

  OrderStatusList list;
  for (int i = 0; i < list.size(); i++) {
    OrderStatus d = list.status(i);
    insertItem(d.index(), d.title());
  }

  setMaxCount(list.size());
  setCurrentIndex(0);

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(setIndex(int)));
}

void OrderStatusBox::setIndex(int i) {
  index = i;
  emit indexChanged(i);
}

int OrderStatusBox::getIndex() { return index; }

StatusDialog::StatusDialog(const QString &current, QWidget *parent)
    : QDialog{parent} {
  setObjectName("status_dialog_box");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Edit Order Status"));
  setMinimumWidth(300);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *info = new QLabel(this);
  info->setText(tr("Update current Order status."));
  layout->addWidget(info);

  m_box = new OrderStatusBox(this);
  int i = m_box->findText(current, Qt::MatchExactly);
  if (i > 0)
    m_box->setCurrentIndex(i);

  layout->addWidget(m_box);

  QDialogButtonBox *btn = new QDialogButtonBox(QDialogButtonBox::Ok);
  layout->addWidget(btn);

  setLayout(layout);

  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
}

int StatusDialog::index() { return m_box->currentIndex(); }
