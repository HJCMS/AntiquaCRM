// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagesdialog.h"
#include "storageeditorwidget.h"
#include "storagetable.h"

StoragesDialog::StoragesDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("storage_location");
  setMinimumSize(700, 400);
  setContentsMargins(0, 0, 0, 0);
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new QToolBar(this);
  m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  ac_add = m_toolBar->addAction(QIcon("://icons/db_add.png"), tr("new entry"));
  ac_add->setStatusTip(tr("insert a new storage location"));
  m_toolBar->addSeparator();
  m_search = new QLineEdit(this);
  m_search->setObjectName("search");
  m_toolBar->addWidget(m_search);
  ac_search =
      m_toolBar->addAction(QIcon("://icons/action-search.png"), tr("search"));
  ac_search->setStatusTip(tr("search in table"));
  m_toolBar->addSeparator();
  layout->addWidget(m_toolBar);

  m_table = new StorageTable(this);
  layout->addWidget(m_table);

  m_editorWidget = new StorageEditorWidget(this);
  layout->addWidget(m_editorWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Save |
                               QDialogButtonBox::Close);

  QPushButton *btn_save = m_btnBox->button(QDialogButtonBox::Save);
  btn_save->setIcon(QIcon("://icons/action-save.png"));

  QPushButton *btn_close = m_btnBox->button(QDialogButtonBox::Close);
  btn_close->setIcon(QIcon("://icons/action-quit.png"));

  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_table, SIGNAL(itemChanged(const StorageItemData &)), m_editorWidget,
          SLOT(setValue(const StorageItemData &)));
  connect(m_table, SIGNAL(queryMessages(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
  connect(ac_add, SIGNAL(triggered()), m_editorWidget, SLOT(clear()));
  connect(ac_search, SIGNAL(triggered()), this, SLOT(searchClicked()));
  connect(m_search, SIGNAL(textChanged(const QString &)), m_table,
          SLOT(findColumn(const QString &)));
  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(saveClicked()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(accept()));
}

void StoragesDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    return;
  }
  QDialog::keyPressEvent(e);
}

bool StoragesDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = reinterpret_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void StoragesDialog::searchClicked() {
  QString str = m_search->text().trimmed();
  if (!str.isEmpty())
    m_table->findColumn(str);
}

void StoragesDialog::saveClicked() {
  QString sql = m_editorWidget->sqlQuery();
  if (sql.isEmpty()) {
    m_statusBar->showMessage(tr("invalid input"), 1000);
    return;
  }
  m_table->postQuery(sql);
}

int StoragesDialog::exec() {
  m_table->initTable();
  m_editorWidget->initDataset();
  return QDialog::exec();
}
