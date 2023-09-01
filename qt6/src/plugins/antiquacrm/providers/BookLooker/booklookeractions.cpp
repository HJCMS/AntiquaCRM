// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookeractions.h"
#include "booklookeroperations.h"

#include <QFrame>
#include <QList>
#include <QRadioButton>

BookLookerActions::BookLookerActions(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("booklooker_actions_dialog");

  QFrame *m_frame = new QFrame(this);
  m_frame->setFrameStyle(QFrame::NoFrame);
  m_frame->setContentsMargins(5, 0, 5, 0);

  int col = 0;
  QVBoxLayout *layout = new QVBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);

  QLabel *m_lb = new QLabel(this);
  m_lb->setWordWrap(false);
  m_lb->setText(tr("Changes the Provider status of the current order."));
  m_lb->setStyleSheet("QLabel {font-weight:bold;}");
  layout->insertWidget(col++, m_lb);

  QLabel *m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QStringList notes;
  notes << tr("Not all service providers fully support these features.");
  notes << tr(
      "Please make sure to check it regularly on the service provider side.");
  m_info->setText(notes.join("<br>"));
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->insertWidget(col++, m_info);

  m_selecter = new BookLookerOperations(this);
  layout->insertWidget(col++, m_selecter);
  layout->setStretch(col, 1);

  m_buyerInfo = new QLabel(this);
  layout->insertWidget(col++, m_buyerInfo);

  m_frame->setLayout(layout);
  m_scrollArea->setWidget(m_frame);

  connect(this, SIGNAL(sendApplyClicked()), SLOT(prepareOperation()));
  connect(this, SIGNAL(sendOkClicked()), SLOT(accept()));
  connect(this, SIGNAL(sendCancelClicked()), SLOT(reject()));
  connect(this, SIGNAL(sendCloseClicked()), SLOT(accept()));
}

void BookLookerActions::prepareOperation() {
  QString action;
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        action = rb->objectName();
        break;
      }
    }
  }

  if (action.isEmpty()) {
    statusMessage(tr("We need a Selection for this operations."));
    return;
  }

  QJsonObject query;
  query.insert("orderId", QJsonValue(order_id));
  query.insert("status", QJsonValue(action));

  // Reset
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_status"));
  obj.insert("query", QJsonValue(query));

  qDebug() << "sendPostOperation" << obj;
  emit sendPostOperation(obj);
}

int BookLookerActions::exec(const QJsonObject &data) {
  QJsonObject _main = data.value("DATA").toObject();
  QJsonObject _order = _main.value("orderinfo").toObject();
  order_id = _order.value("o_provider_order_id").toString();
  // qDebug() << _order << order_id;
  if (order_id.isEmpty()) {
    qWarning("Missing OrderId - Operation rejected!");
    return QDialog::Rejected;
  }

  QString _info = tr("Create remote action for %1 with Order Id: %2")
                      .arg(_main.value("buyer").toString()
                      .arg(order_id));
  m_buyerInfo->setText(_info);

  setWindowModified(false);
  return AntiquaCRM::ProviderActionDialog::exec();
}
