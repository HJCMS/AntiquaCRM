// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorysubject.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>

#include <SqlCore>

CategorySubjectBox::CategorySubjectBox(QWidget *parent) : QComboBox{parent} {
  setInsertPolicy(QComboBox::NoInsert);
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  setEditable(false);
}

static const QString queryProviderSubject() {
  QString sql("SELECT DISTINCT cs_id,cs_identifier");
  sql.append(" FROM ref_category_subjects");
  sql.append(" WHERE cs_section=0 ORDER BY cs_id;");
  return sql;
}

CategorySubject::CategorySubject(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_box = new CategorySubjectBox(this);
  layout->addWidget(m_box);

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Search"));
  m_search->setClearButtonEnabled(true);
  layout->addWidget(m_search);

  QToolButton *m_help = new QToolButton(this);
  m_help->setIcon(myIcon("info"));
  m_help->setToolTip(tr("Open Helper Dialog"));
  layout->addWidget(m_help);

  setLayout(layout);
  setRequired(true);
  setModified(false);

  connect(m_help, SIGNAL(clicked()), this, SLOT(openHelperDialog()));

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(subjectChanged(int)));

  connect(m_search, SIGNAL(textChanged(const QString &)), this,
          SLOT(searchChanged(const QString &)));
}

void CategorySubject::subjectChanged(int index) {
  if (index < 0)
    return;

  setModified(true);
}

void CategorySubject::searchChanged(const QString &search) {
  if (search.length() < 5)
    return;

  int index = m_box->findText(search, Qt::MatchStartsWith);
  if (index > 0)
    m_box->setCurrentIndex(index);
}

void CategorySubject::openHelperDialog() {
  QString helpText = tr(
      "In this area, the categories are defined which are permanently assigned "
      "by your service provider. Ultimately, it is decided here in which "
      "category your book is for sale.");
  QMessageBox::information(this, info(), helpText);
}

void CategorySubject::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void CategorySubject::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void CategorySubject::setValue(const QVariant &val) {
  int index = m_box->findData(val.toInt(), Qt::UserRole);
  if (index >= 0) {
    m_box->setCurrentIndex(index);
    setModified(true);
  }
}

void CategorySubject::findIndex(const QString &match) {
  int index = m_box->findText(match, Qt::MatchStartsWith);
  if (index > 0)
    m_box->setCurrentIndex(index);
}

void CategorySubject::loadDataset() {
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QSqlQuery q = m_sql->query(queryProviderSubject());
  if (q.size() > 1) {
    m_box->clear();
    while (q.next()) {
      m_box->addItem(q.value("cs_identifier").toString(), q.value("cs_id"));
    }
  } else {
    if (!m_sql->lastError().isEmpty()) {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
    }
  }
}

const QVariant CategorySubject::value() {
  return m_box->itemData(m_box->currentIndex(), Qt::UserRole);
}

bool CategorySubject::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void CategorySubject::setInfo(const QString &info) { m_box->setToolTip(info); }

const QString CategorySubject::info() { return m_box->toolTip(); }

const QString CategorySubject::notes() {
  return tr("a valid Provider Category is required!");
}
