// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoryedit.h"
#include "categorytree.h"
#include "draglistwidget.h"
#include "myicontheme.h"

#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>

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
  m_btnBox->setStandardButtons(QDialogButtonBox::Close);
  QPushButton *m_toggleBtn = new QPushButton(m_btnBox);
  m_toggleBtn->setText(tr("Treeview"));
  m_toggleBtn->setToolTip(tr("Open/close Treeview"));
  m_toggleBtn->setIcon(myIcon("view_choose"));
  m_btnBox->addButton(m_toggleBtn, QDialogButtonBox::ResetRole);
  m_saveBtn = m_btnBox->addButton(QDialogButtonBox::Apply);
  m_saveBtn->setText(tr("Save"));
  m_saveBtn->setIcon(myIcon("filesave"));
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1); // Splitter row
  setLayout(layout);

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_saveBtn, SIGNAL(clicked()), this, SLOT(saveCompanyTreeUsage()));
  connect(m_toggleBtn, SIGNAL(clicked()), m_tree, SLOT(toggleTreeView()));
  connect(m_tree, SIGNAL(sendCompanyUsage(int, bool)), this,
          SLOT(updateCompanyUsage(int, bool)));
  connect(m_tree, SIGNAL(sendDisableUsageList(const QStringList &)), this,
          SLOT(disableCompanyUsageList(const QStringList &)));
  connect(m_tree, SIGNAL(sendListItemHidden(const QString &)), m_storageList,
          SLOT(setItemHidden(const QString &)));
  connect(m_tree, SIGNAL(sendListItemHidden(const QString &)), m_keywordsList,
          SLOT(setItemHidden(const QString &)));
  connect(m_tree, SIGNAL(sendListItemVisible(const QString &)), m_storageList,
          SLOT(setItemVisible(const QString &)));
  connect(m_tree, SIGNAL(sendListItemVisible(const QString &)), m_keywordsList,
          SLOT(setItemVisible(const QString &)));
}

bool CategoryEdit::initCategories() {
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
    if (!m_sql->lastError().isEmpty()) {
      qDebug() << m_sql->lastError();
      return false;
    }
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
    if (!m_sql->lastError().isEmpty()) {
      qDebug() << m_sql->lastError();
      return false;
    }
  }
  return true;
}

bool CategoryEdit::initKeywords() {
  QStringList storageKeys;
  // Storage
  QString sql("SELECT sl_identifier AS n FROM ref_storage_location");
  sql.append(" WHERE sl_id>0;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString w = q.value("n").toString();
      if (!storageKeys.contains(w))
        storageKeys.append(w);
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      qDebug() << m_sql->lastError();
      return false;
    }
  }
  if (storageKeys.size() > 1) {
    storageKeys.sort(Qt::CaseSensitive);
    m_storageList->addItems(storageKeys);
  }

  QStringList keywordKeys;
  sql = QString("SELECT ci_name AS n FROM categories_intern");
  sql.append(" ORDER BY ci_id;");
  q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString w = q.value("n").toString();
      if (!keywordKeys.contains(w) && !storageKeys.contains(w))
        keywordKeys.append(w);
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      qDebug() << m_sql->lastError();
      return false;
    }
  }
  if (keywordKeys.size() > 1) {
    keywordKeys.sort(Qt::CaseSensitive);
    m_keywordsList->addItems(keywordKeys);
  }
  keywordKeys.clear();
  storageKeys.clear();
  return true;
}

void CategoryEdit::saveCompanyTreeUsage() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CategoryEdit::updateCompanyUsage(int categoryId, bool usage) {
  QString sql("UPDATE categories_extern SET ce_company_usage=");
  sql.append((usage) ? "true" : "false");
  sql.append(" WHERE ce_id=" + QString::number(categoryId) + ";");
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
    qDebug() << m_sql->lastError() << sql;
    m_statusBar->showMessage(tr("An error has occurred!"), timeout);
  } else {
    m_statusBar->showMessage(tr("Database Update successfully!"), timeout);
  }
}

void CategoryEdit::disableCompanyUsageList(const QStringList &ids) {
  QString idList = ids.join(",");
  QString sql("UPDATE categories_extern SET ce_company_usage=");
  sql.append("false WHERE ce_id IN (" + idList + ");");
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
    qDebug() << m_sql->lastError() << sql;
    m_statusBar->showMessage(tr("An error has occurred!"), timeout);
  } else {
    m_statusBar->showMessage(tr("Database Update successfully!"), timeout);
  }
}

int CategoryEdit::exec() {
  if (initCategories())
    initKeywords();

  return QDialog::exec();
}
