// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordsdialog.h"
#include "keywordsedit.h"
#include "keywordstable.h"

KeywordsDialog::KeywordsDialog(QWidget *parent)
    : AntiquaCRM::ADialog{parent}, p_queryTable{} {
  setWindowTitle(tr("Keywords editor") + " [*]");
  setWindowIcon(AntiquaCRM::antiquaIcon("document-edit"));

  m_table = new KeywordsTable(this);
  m_table->setObjectName("query_table_view");
  layout->addWidget(m_table);
  layout->setStretch(0, 1);

  m_edit = new KeywordsEdit(this);
  layout->addWidget(m_edit);

  // ButtonsBar
  btn_reject->setText(tr("Close"));
  btn_reject->setIcon(AntiquaCRM::antiquaIcon("action-quit"));

  btn_commit = m_buttonsBar->addButton(QDialogButtonBox::Apply);
  btn_commit->setIcon(AntiquaCRM::antiquaIcon("action-save"));

  btn_create = m_buttonsBar->addButton(QDialogButtonBox::Reset);
  btn_create->setText(tr("Clear"));
  btn_create->setIcon(AntiquaCRM::antiquaIcon("action-add"));

  // Signals
  connect(btn_create, SIGNAL(clicked()), m_edit, SLOT(clear()));
  connect(btn_commit, SIGNAL(clicked()), SLOT(commit()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
  connect(m_table, SIGNAL(signalRowSelected(const QSqlRecord &)), m_edit,
          SLOT(setData(const QSqlRecord &)));

  connect(m_table, SIGNAL(signalFound(qint64)), m_edit, SLOT(changeId(qint64)));

  connect(m_edit, SIGNAL(search(const QString &)), m_table,
          SLOT(find(const QString &)));
}

const QString KeywordsDialog::tableName() const {
  return QString("categories_intern");
}

void KeywordsDialog::commit() {
  QSqlRecord _r = m_edit->getData(tableName());
  if (_r.count() > 2) {
    if (m_table->commitQuery(_r))
      m_statusBar->showMessage(tr("Update Success"), 5000);
    else
      m_statusBar->showMessage(tr("Update failed!"), 10000);
  }
}

int KeywordsDialog::exec() {
  if (p_queryTable.isEmpty())
    m_table->initTable(tableName());

  m_table->select();
  return QDialog::exec();
}
