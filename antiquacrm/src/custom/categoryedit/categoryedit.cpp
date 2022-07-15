// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoryedit.h"
#include "categorytree.h"
#include "draglistwidget.h"

#include <QtCore>
#include <QtWidgets>

CategoryEdit::CategoryEdit(QWidget *parent) : QDialog{parent} {
  setObjectName("category_edit");
  setWindowTitle(tr("Edit Category Views"));
  setMinimumSize(QSize(800, 560));
  setSizeGripEnabled(true);

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  QString mainInfo = tr("You can use the mouse to drag and drop keywords and "
                        "categories into this area.");
  layout->addWidget(new QLabel(mainInfo, this));

  m_splitter = new QSplitter(Qt::Horizontal, this);
  m_tree = new CategoryTree(m_splitter);
  m_splitter->addWidget(m_tree);

  QFrame *m_frame = new QFrame(m_splitter);
  QVBoxLayout *frameLayout = new QVBoxLayout(m_frame);
  frameLayout->addWidget(new QLabel(tr("Storage Categories"), m_frame));
  m_storageList = new DragListWidget(m_frame);
  frameLayout->addWidget(m_storageList);

  frameLayout->addWidget(new QLabel(tr("Company Keywords"), m_frame));
  m_keywordsList = new DragListWidget(m_frame);
  frameLayout->addWidget(m_keywordsList);

  m_splitter->addWidget(m_frame);
  m_splitter->setStretchFactor(0, 60);
  m_splitter->setStretchFactor(1, 40);
  layout->addWidget(m_splitter);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1); // Splitter row
  setLayout(layout);

  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_tree, SIGNAL(sendCompanyUsage(int, bool)), this,
          SLOT(updateCompanyUsage(int, bool)));
  connect(m_tree, SIGNAL(sendDisableUsageList(const QStringList &)), this,
          SLOT(disableCompanyUsageList(const QStringList &)));
}

void CategoryEdit::initCategories() {
  // Main Categories
  QString sql("SELECT ce_id, ce_name FROM categories_extern");
  sql.append(" WHERE ce_depth='0' AND ");
  sql.append("ce_provider_name!='Internal' ORDER BY ce_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString topName = q.value("ce_name").toString();
      QTreeWidgetItem *parent = m_tree->addTopLevel(topName);
      parent->setData(0, Qt::UserRole, q.value("ce_id").toInt());
    }
  } else {
    qDebug() << m_sql->lastError();
  }
  /* Sub Categories
   * ce_id ce_depth ce_binding ce_name ce_provider_name ce_company_usage
   */
  sql = QString("SELECT * FROM categories_extern WHERE");
  sql.append(" ce_depth='1' AND ");
  sql.append("ce_provider_name!='Internal' ORDER BY ce_name;");
  q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int bindId = q.value("ce_binding").toInt();
      if (bindId < 1)
        continue;

      QString title = q.value("ce_name").toString();
      bool status = q.value("ce_company_usage").toBool();
      QTreeWidgetItem *subItem = m_tree->addSubLevel(title, bindId, status);
      if (subItem != nullptr) {
        subItem->setData(0, Qt::UserRole, q.value("ce_id").toInt());
      }
    }
  } else {
    qDebug() << m_sql->lastError();
  }
}

void CategoryEdit::initKeywords() {
  QStringList words;
  QString sql("SELECT ci_name AS n FROM categories_intern");
  sql.append(" ORDER BY ci_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString w = q.value("n").toString();
      if (!words.contains(w))
        words.append(w);
    }
  } else {
    if (!m_sql->lastError().isEmpty())
      qDebug() << m_sql->lastError();
  }
  if (words.size() > 1) {
    words.sort(Qt::CaseSensitive);
    m_keywordsList->addItems(words);
  }
  // Storage
  words.clear();
  sql = QString("SELECT sl_identifier AS n FROM ref_storage_location");
  sql.append(" WHERE sl_id>0;");
  q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString w = q.value("n").toString();
      if (!words.contains(w))
        words.append(w);
    }
  } else {
    if (!m_sql->lastError().isEmpty())
      qDebug() << m_sql->lastError();
  }
  if (words.size() > 1) {
    words.sort(Qt::CaseSensitive);
    m_storageList->addItems(words);
  }
}

void CategoryEdit::updateCompanyUsage(int categoryId, bool usage) {
  QString sql("UPDATE categories_extern SET ce_company_usage=");
  sql.append((usage) ? "true" : "false");
  sql.append(" WHERE ce_id=" + QString::number(categoryId) + ";");
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
    qDebug() << m_sql->lastError() << sql;
    m_statusBar->showMessage(tr("An error has occurred!"), 6000);
  } else {
    m_statusBar->showMessage(tr("Database Update successfully!"), 6000);
  }
}

void CategoryEdit::disableCompanyUsageList(const QStringList &ids) {
  QString idList = ids.join(",");
  QString sql("UPDATE categories_extern SET ce_company_usage=");
  sql.append("false WHERE ce_id IN (" + idList + ");");
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
    qDebug() << m_sql->lastError() << sql;
    m_statusBar->showMessage(tr("An error has occurred!"), 6000);
  } else {
    m_statusBar->showMessage(tr("Database Update successfully!"), 6000);
  }
}

int CategoryEdit::exec() {
  initCategories();
  initKeywords();

  return QDialog::exec();
}
