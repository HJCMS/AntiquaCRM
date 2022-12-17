// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksdialog.h"

#include <QLabel>
#include <QLayout>

AbeBooksDialog::AbeBooksDialog(QWidget *parent)
    : AntiquaCRM::UpdateDialog{parent} {
  setMinimumWidth(450);
  setWindowTitle("AbeBooks");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  int col = 0;
  QLabel *m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QStringList notes;
  notes << QString("This Plugin currently not support any remote actions.");
  m_info->setText(notes.join("<br>"));
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->insertWidget(col++, m_info);

  setLayout(layout);
}

bool AbeBooksDialog::prepareAction() { return false; }

void AbeBooksDialog::networkAction() {}

int AbeBooksDialog::exec(const QString &orderId, const QJsonObject &data) {
  p_order_id = orderId;
  p_system_data = data;
  return QDialog::exec();
}
