// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publisherdialog.h"
#include "publisheredit.h"
#include "publishertableview.h"

PublisherDialog::PublisherDialog(QWidget *parent)
    : AntiquaCRM::ADialog{parent} {
  setObjectName("publisher_dialog");
  setWindowTitle(tr("Publishers edit") + "[*]");

  m_table = new PublisherTableView(this);
  layout->addWidget(m_table);

  m_edit = new PublisherEdit(this);
  layout->addWidget(m_edit);

  connect(m_table, SIGNAL(sendItemSelected(const QModelIndex &)),
          SLOT(itemSelected(const QModelIndex &)));
  connect(m_edit, SIGNAL(sendDataChanged()), SLOT(dataChanged()));
  connect(btn_apply, SIGNAL(clicked()), SLOT(saveData()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
}

void PublisherDialog::itemSelected(const QModelIndex &index) {
  p_tmp.first = m_table->getPublisher(index);
  p_tmp.second = m_table->getLocation(index);
  m_edit->setData(p_tmp.first, p_tmp.second);
}

void PublisherDialog::dataChanged() {
  setWindowModified(true);
  btn_apply->setEnabled(isWindowModified());
}

void PublisherDialog::saveData() {
  const QString _publisher = m_edit->getPublisher();
  const QString _location = m_edit->getLocation();
  if (_publisher.length() < 2 || _location.length() < 2) {
    m_statusBar->showMessage(tr("Missing data ..."), (10 * 1000));
    return;
  }

  if (m_table->saveData(_publisher, _location)) {
    setWindowModified(false);
    m_statusBar->showMessage(tr("Successfully saved"));
    m_table->setReloadView();
  } else {
    m_statusBar->showMessage(tr("Not saved"));
  }
  btn_apply->setEnabled(isWindowModified());
}

int PublisherDialog::exec() {
  if (!m_table->init())
    return QDialog::Rejected;

  return AntiquaCRM::ADialog::exec();
}
