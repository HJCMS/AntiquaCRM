// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "completerview.h"
#include "addentrydialog.h"
#include "antiqua_global.h"
#include "datadelegation.h"
#include "datatablemodel.h"
#include "messagebox.h"
#include "myicontheme.h"
#include "sqlcore.h"

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QSqlRecord>

CompleterView::CompleterView(const QString &filter, QWidget *parent)
    : QTableView{parent}, p_type(filter) {
  setObjectName("completer_view_table");
  setEditTriggers(QAbstractItemView::DoubleClicked);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setAlternatingRowColors(true);
  setSortingEnabled(true);
  setSelectionMode(QAbstractItemView::SingleSelection);

  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  p_db = m_sql->db();

  m_model = new DataTableModel(p_type, this, p_db);
  if (m_model->select())
    setModel(m_model);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setSectionHidden(0, true);  /**< k_table_cell */
  tHeader->setSectionHidden(1, true);  /**< k_type */
  tHeader->setSectionHidden(2, false); /**< k_keyword */
  tHeader->setSectionHidden(3, false); /**< k_description */
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
  tHeader->setStretchLastSection(true);
  tHeader->setSortIndicatorShown(true);
  tHeader->setCascadingSectionResizes(true);

  m_addShortcut = new QShortcut(this);
  m_addShortcut->setAutoRepeat(false);
  m_addShortcut->setContext(Qt::WidgetShortcut);
  m_addShortcut->setKey(QKeySequence(Qt::CTRL + Qt::Key_N));

  connect(m_model, SIGNAL(s_sqlError(const QString &)), this,
          SLOT(errorPopUp(const QString &)));
  connect(m_addShortcut, SIGNAL(activated()), this, SLOT(createNewEntry()));
}

void CompleterView::errorPopUp(const QString &err) {
  MessageBox messanger(this);
  messanger.failed(tr("SQL Error"), err);
}

void CompleterView::createNewEntry() {
  QSqlRecord rec = m_model->record();
  rec.clearValues();
  rec.setValue("k_table_cell", p_type);

  AddEntryDialog dialog;
  if (dialog.exec()) {
    QString keyword = dialog.keyword();
    if (keyword.isEmpty())
      return;

    rec.setValue("k_keyword", keyword);
    rec.setValue("k_description", dialog.description());
    m_model->insertSqlQuery(rec);
  } else {
    QString err = p_db.lastError().text();
    qDebug() << Q_FUNC_INFO << err;
    errorPopUp(err);
  }
}

void CompleterView::removeSelection() {
  QMessageBox::StandardButton ret = QMessageBox::information(
      this, tr("Entry delete"), tr("Do you really want to delete this entry?"),
      (QMessageBox::Ok | QMessageBox::Cancel));

  if (ret != QMessageBox::Ok)
    return;

  QModelIndex index = currentIndex();
  if (index.isValid()) {
    m_model->removeData(index);
  }
}

void CompleterView::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu(tr("Actions"), this);
  QAction *add = m->addAction(myIcon("db_add"), tr("Add new entry"));
  add->setShortcut(m_addShortcut->key());
  connect(add, SIGNAL(triggered()), this, SLOT(createNewEntry()));
  QAction *del = m->addAction(myIcon("db_remove"), tr("Remove entry"));
  connect(del, SIGNAL(triggered()), this, SLOT(removeSelection()));
  m->exec(ev->globalPos());
  delete m;
}
