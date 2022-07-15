// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorysubject.h"
#include "myicontheme.h"
#include <SqlCore>

#include <QCompleter>
#include <QDebug>
#include <QHBoxLayout>
#include <QJsonValue>
#include <QMessageBox>
#include <QMutex>

/**
 * @brief Standard Feldauswahl
 * @note Ist bei allen Abfragen gleich!
 * @return QUERY_STATEMENT
 */
static const QString querySelect() {
  QString sql("SELECT DISTINCT");
  sql.append(" ce_id AS id,");
  sql.append(" ce_name AS category,");
  sql.append(" ce_binding AS binding");
  sql.append(" FROM categories_extern");
  return sql;
}

CategorySubjectBox::CategorySubjectBox(QWidget *parent) : QComboBox{parent} {
  setInsertPolicy(QComboBox::NoInsert);
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  setMinimumWidth(150);
  setEditable(false);
}

CategorySubject::CategorySubject(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_sql = new HJCMS::SqlCore(this);

  m_boxMain = new CategorySubjectBox(this);
  m_boxMain->setToolTip(tr("Main Category"));
  layout->addWidget(m_boxMain);

  m_boxSub = new CategorySubjectBox(this);
  m_boxSub->setToolTip(tr("Sub Category"));
  layout->addWidget(m_boxSub);

  m_syncBtn = new QPushButton(tr("sync"), this);
  m_syncBtn->setIcon(myIcon("rebuild"));
  m_syncBtn->setToolTip(tr("synchronize categories"));
  layout->addWidget(m_syncBtn);

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Search"));
  m_search->setClearButtonEnabled(true);
  layout->addWidget(m_search);

  m_helpBtn = new QToolButton(this);
  m_helpBtn->setIcon(myIcon("info"));
  m_helpBtn->setToolTip(tr("Open Information Dialog"));
  layout->addWidget(m_helpBtn);

  setLayout(layout);
  setRequired(true);
  setModified(false);

  connect(m_syncBtn, SIGNAL(clicked()), this, SLOT(syncronizeClicked()));
  connect(m_helpBtn, SIGNAL(clicked()), this, SLOT(openHelperDialog()));
  connect(m_boxMain, SIGNAL(currentIndexChanged(int)), this,
          SLOT(mainCategoryChanged(int)));

  connect(m_search, SIGNAL(textChanged(const QString &)), this,
          SLOT(searchChanged(const QString &)));
}

void CategorySubject::setMainCategories() {
  QString sql(querySelect());
  sql.append(" WHERE ce_depth='0' AND ce_company_usage=true");
  sql.append(" ORDER BY ce_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 1) {
    m_boxMain->clear();
    while (q.next()) {
      m_boxMain->addItem(q.value("category").toString(), q.value("id"));
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      sqlErrorPopUp(m_sql->lastError());
    }
  }
}

void CategorySubject::setCompleter(const QStringList &list) {
  QMutex mutex(QMutex::Recursive);
  mutex.lock();
  QCompleter *cpl = new QCompleter(list, this);
  cpl->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  cpl->setCaseSensitivity(Qt::CaseInsensitive);
  cpl->setFilterMode(Qt::MatchStartsWith);
  m_search->setCompleter(cpl);
  mutex.unlock();
}

void CategorySubject::setSubCategories(int ceId) {
  if (ceId == 0)
    return;

  QString ce_id = QString::number(ceId);
  QString sql(querySelect());
  sql.append(" WHERE ce_depth='1' AND ce_company_usage=true");
  sql.append(" AND ce_binding=" + ce_id);
  sql.append(" ORDER BY ce_id;");
  QSqlQuery q = m_sql->query(sql);
  // qDebug() << Q_FUNC_INFO << sql << ce_id;
  if (q.size() > 0) {
    m_boxSub->clear();
    while (q.next()) {
      m_boxSub->addItem(q.value("category").toString(), q.value("binding"));
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      sqlErrorPopUp(m_sql->lastError());
      return;
    }
  }
  // Wenn nicht Leer dann auswahl suchen
  QString searchText = m_search->text().trimmed();
  if (searchText.length() > 3) {
    int index = m_boxSub->findData(searchText, Qt::DisplayRole);
    m_boxSub->setCurrentIndex(index);
  }
}

void CategorySubject::sqlErrorPopUp(const QString &err) {
  QMessageBox *errBox = new QMessageBox(this);
  errBox->setMinimumSize(QSize(400, 100));
  errBox->setText(tr("A database error has occurred."));
  errBox->setTextFormat(Qt::PlainText);
  errBox->setInformativeText(err);
  errBox->setStandardButtons(QMessageBox::Ok);
  errBox->exec();
}

void CategorySubject::mainCategoryChanged(int index) {
  int ce_id = m_boxMain->itemData(index, Qt::UserRole).toInt();
  if (ce_id < 1)
    return;

  setSubCategories(ce_id);
  setModified(true);
}

void CategorySubject::searchChanged(const QString &search) {
  if (search.length() > 3) {
    QStringList list;
    QString sql(querySelect());
    sql.append(" WHERE ce_depth='1' AND ce_name ILIKE '%");
    sql.append(search);
    sql.append("%' AND ce_company_usage=true;");
    QSqlQuery q = m_sql->query(sql);
    if (q.size() > 0) {
      while (q.next()) {
        list.append(q.value("category").toString());
      }
    } else {
      if (!m_sql->lastError().isEmpty()) {
        sqlErrorPopUp(m_sql->lastError());
        return;
      }
    }
    if (list.count() > 1)
      setCompleter(list);

    list.clear();
  }
}

void CategorySubject::syncronizeClicked() {
  QString search = m_search->text().trimmed();
  if (search.length() < 3)
    return;

  QString sql(querySelect());
  sql.append(" WHERE ce_company_usage=true AND ce_depth='1'");
  sql.append(" AND ce_name='" + search + "';");
  QSqlQuery q = m_sql->query(sql);
  int found = -1;
  if (q.size() > 0) {
    while (q.next()) {
      qDebug() << Q_FUNC_INFO << q.value("binding") <<  q.value("category");
      found = q.value("binding").toInt();
      // break;
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      sqlErrorPopUp(m_sql->lastError());
      return;
    }
  }
  if (found > 0) {
    int index = m_boxMain->itemData(found, Qt::UserRole).toInt();
    m_boxMain->setCurrentIndex(index);
  }
}

void CategorySubject::openHelperDialog() {
  QString helpText = tr(
      "In this area, the categories are defined which are permanently assigned "
      "by your service provider. Ultimately, it is decided here in which "
      "category your book is for sale.");
  QMessageBox::information(this, info(), helpText);
}

void CategorySubject::reset() {
  m_boxMain->setCurrentIndex(0);
  setModified(false);
}

void CategorySubject::setFocus() {
  m_boxMain->setFocus();
  m_boxMain->showPopup();
}

void CategorySubject::setValue(const QVariant &val) {
  int mainId = val.toInt();
  int index = m_boxMain->findData(mainId, Qt::UserRole);
  if (index >= 0) {
    m_boxMain->setCurrentIndex(index);
    setModified(true);
  }
}

void CategorySubject::findIndex(const QString &match) {
  int index = m_boxMain->findText(match, Qt::MatchStartsWith);
  if (index > 0) {
    m_boxMain->setCurrentIndex(index);
  }
}

void CategorySubject::loadDataset() { setMainCategories(); }

const QVariant CategorySubject::value() {
  QJsonObject obj;
  int mIndex = m_boxMain->currentIndex();
  QString mainCategory = m_boxMain->currentText().trimmed();
  obj.insert("main", QJsonValue(mainCategory));
  int sIndex = m_boxMain->currentIndex();
  if (sIndex == -1) {
    obj.insert("sub", QJsonValue(QString()));
  } else {
    QString subCategory = m_boxSub->currentText().trimmed();
    obj.insert("sub", QJsonValue(subCategory));
  }
  qDebug() << Q_FUNC_INFO << obj;

  int mainId = m_boxMain->itemData(mIndex, Qt::UserRole).toInt();
  return mainId;
}

bool CategorySubject::isValid() {
  if (isRequired() && m_boxMain->currentIndex() == 0)
    return false;

  return true;
}

void CategorySubject::setInfo(const QString &info) {
  m_search->setToolTip(info);
}

const QString CategorySubject::info() { return m_search->toolTip(); }

const QString CategorySubject::notes() {
  return tr("a valid Provider Category is required!");
}
