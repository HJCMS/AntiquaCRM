// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "completertable.h"
#include "datadelegation.h"
#include "datatablemodel.h"
#include "applsettings.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QVBoxLayout>

CompleterTable::CompleterTable(QWidget *parent, const QString &filter)
    : QDialog{parent}, p_db(QSqlDatabase::database(ApplSettings::sqlConnectioName())) {
  setObjectName("CompleterTableDialog");
  setSizeGripEnabled(true);
  setMinimumSize(QSize(650, 400));

  if (filter.contains("ib_condition"))
    setWindowTitle(tr("Modify Edition dataset"));
  else if (filter.contains("ib_designation"))
    setWindowTitle(tr("Modify Designation dateset"));
  else
    setWindowTitle(tr("Editing Keyword Tables"));

  if (!p_db.isValid())
    qFatal("Missing Database connection");

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName("compliterlayout");

  QLabel *info = new QLabel(this);
  info->setText(tr("Please enter the keywords here that should be suggested in "
                   "the editor."));
  info->setWordWrap(true);
  verticalLayout->addWidget(info);

  m_tableView = new QTableView(this);
  m_tableView->setObjectName("compliter_table");
  m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
  m_tableView->setCornerButtonEnabled(false);
  m_tableView->setDragEnabled(false);
  m_tableView->setDragDropOverwriteMode(false);
  m_tableView->setAlternatingRowColors(true);
  m_tableView->setSortingEnabled(true);
  m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  verticalLayout->addWidget(m_tableView);

  m_model = new DataTableModel(filter, m_tableView, p_db);
  if (m_model->select())
    m_tableView->setModel(m_model);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = m_tableView->horizontalHeader();
  tHeader->setSectionHidden(0, true);  /**< k_table_cell */
  tHeader->setSectionHidden(1, false); /**< k_keyword */
  tHeader->setSectionHidden(2, false); /**< k_description */
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
  tHeader->setStretchLastSection(true);
  tHeader->setSortIndicatorShown(true);
  tHeader->setCascadingSectionResizes(true);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  verticalLayout->addWidget(m_buttonBox);

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setObjectName("CloseEditTableDialog");
  m_close->setIcon(myIcon("exit"));
  m_close->setShortcut(QKeySequence::Close);

  setLayout(verticalLayout);

  m_addShortcut = new QShortcut(this);
  m_addShortcut->setAutoRepeat(false);
  m_addShortcut->setContext(Qt::WidgetShortcut);
  m_addShortcut->setWhatsThis(tr("add new entry"));
  m_addShortcut->setKey(QKeySequence(Qt::CTRL + Qt::Key_N));

  m_delShortcut = new QShortcut(this);
  m_delShortcut->setAutoRepeat(false);
  m_delShortcut->setContext(Qt::WidgetShortcut);
  m_delShortcut->setWhatsThis(tr("remove entry"));
  m_delShortcut->setKey(QKeySequence(Qt::CTRL + Qt::Key_D));

  connect(m_addShortcut, SIGNAL(activated()), m_model, SLOT(insertEntry()));
  connect(m_delShortcut, SIGNAL(activated()), this,
          SLOT(removeSelectedEntries()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  connect(m_model, SIGNAL(s_dataChanged()), m_tableView,
          SLOT(resizeRowsToContents()));
}

void CompleterTable::removeSelectedEntries() {
  QMessageBox::StandardButton ret = QMessageBox::information(
      this, tr("Entry delete"), tr("Do you really want to delete this entry?"),
      (QMessageBox::Ok | QMessageBox::Cancel));

  if (ret != QMessageBox::Ok)
    return;

  QModelIndex index = m_tableView->currentIndex();
  if (index.isValid()) {
    m_model->removeData(index);
  }
}

void CompleterTable::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu(tr("Actions"), this);
  QAction *add = m->addAction(myIcon("db_add"), tr("add new entry"));
  add->setShortcut(m_addShortcut->key());
  connect(add, SIGNAL(triggered()), m_model, SLOT(insertEntry()));
  QAction *del = m->addAction(myIcon("db_remove"), tr("remove entry"));
  del->setShortcut(m_delShortcut->key());
  connect(del, SIGNAL(triggered()), this, SLOT(removeSelectedEntries()));
  m->exec(ev->globalPos());
  delete m;
}
