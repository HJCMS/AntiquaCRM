// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcode.h"
// Privat
#include <SqlCore>
// Qt
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

PostalCodeModel::PostalCodeModel(const QString &table, QObject *parent)
    : QSqlQueryModel{parent} {
  setObjectName("PostalCodeDataModel");
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  setQuery("SELECT DISTINCT p_plz FROM " + table + " ORDER BY p_plz;",
           m_sql->db());
}

PostalCode::PostalCode(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("PostalCodeEditor");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  layout->addWidget(m_info);

  m_plz = new QLineEdit(this);
  m_plz->setMaxLength(10);
  m_plz->setMaximumWidth(100);
  m_plz->setObjectName("plz_edit");
  layout->addWidget(m_plz);

  m_completer = new QCompleter(m_plz);
  m_completer->setMaxVisibleItems(20);
  m_plz->setCompleter(m_completer);

  m_country = new QComboBox(this);
  m_country->setToolTip(tr("currently supported helper functions"));
  m_country->insertItem(0, tr("German"), QVariant("ref_postalcodes_de"));
  m_country->insertItem(1, tr("Austria"), QVariant("ref_postalcodes_at"));
  layout->addWidget(m_country);

  setLayout(layout);

  connect(m_country, SIGNAL(currentIndexChanged(int)), this,
          SLOT(loadDataset(int)));

  connect(m_plz, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
}

bool PostalCode::setCompleterModel(const QString &table) {
  PostalCodeModel *m_model = new PostalCodeModel(table, m_completer);
  m_completer->setModel(m_model);
  return true;
}

void PostalCode::loadDataset(int) {
  QString table =
      m_country->itemData(m_country->currentIndex(), Qt::UserRole).toString();
  if (table.isEmpty())
    return;

  if (setCompleterModel(table)) {
    setModified(true);
  }
}

void PostalCode::setValue(const QVariant &val) {
  m_plz->setText(val.toString());
  loadDataset(0);
}

void PostalCode::reset() {
  m_plz->clear();
  setModified(false);
}

const QVariant PostalCode::value() { return m_plz->text(); }

bool PostalCode::isValid() {
  if (isRequired() && m_plz->text().isEmpty())
    return false;

  return true;
}

void PostalCode::setInfo(const QString &info) {
  m_plz->setToolTip(info);
  QString label(info);
  label.append(":");
  m_info->setText(label);
}

const QString PostalCode::info() { return m_info->text(); }

const QString PostalCode::selectedTableName() {
  int i = m_country->currentIndex();
  QVariant t = m_country->itemData(i, Qt::UserRole).toString();
  return t.toString();
}

const QString PostalCode::notes() {
  return tr("a valid Postalcode is required!");
}
