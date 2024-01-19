// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagesdialog.h"
#include "storageeditorwidget.h"
#include "storagesearchbar.h"
#include "storagetable.h"

StoragesDialog::StoragesDialog(QWidget *parent) : AntiquaCRM::ADialog{parent} {
  setObjectName("storage_location");
  setWindowTitle(tr("Storage editor") + "[*]");
  setMinimumSize(700, 400);

  m_table = new StorageTable(this);
  layout->addWidget(m_table);

  m_editorWidget = new StorageEditorWidget(this);
  m_editorWidget->installEventFilter(this);
  layout->addWidget(m_editorWidget);

  m_searchBar = new StorageSearchBar(this);
  layout->addWidget(m_searchBar);

  btn_create = new QPushButton(tr("Create"), m_buttonsBar);
  btn_create->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  btn_create->setToolTip(tr("Create a new storage entry"));
  btn_create->setStatusTip(btn_create->toolTip());
  m_buttonsBar->addButton(btn_create, QDialogButtonBox::ActionRole);
  btn_apply->setEnabled(true);

  connect(m_table, SIGNAL(itemChanged(const StorageItemData &)), m_editorWidget,
          SLOT(setValue(const StorageItemData &)));

  connect(m_table, SIGNAL(queryMessages(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(tableRecordChanged(const QSqlRecord &)),
          m_editorWidget, SLOT(setRestrictions(const QSqlRecord &)));

  connect(m_searchBar, SIGNAL(sendFindStorage(const QString &)), m_table,
          SLOT(findColumn(const QString &)));

  connect(btn_create, SIGNAL(clicked()), m_editorWidget, SLOT(clear()));
  connect(btn_apply, SIGNAL(clicked()), SLOT(save()));
  connect(m_buttonsBar, SIGNAL(rejected()), SLOT(accept()));
}

void StoragesDialog::save() {
  QString _sql = m_editorWidget->sqlQuery();
  if (_sql.isEmpty()) {
    m_statusBar->showMessage(tr("invalid input"), 1000);
    return;
  }

  if (m_table->startQuery(_sql))
    setWindowModified(false);
}

int StoragesDialog::exec() {
  m_table->initTable();
  m_editorWidget->initDataset();
  return QDialog::exec();
}
