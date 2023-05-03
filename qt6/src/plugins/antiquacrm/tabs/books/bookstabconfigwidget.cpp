// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstabconfigwidget.h"
#include "booksconfig.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>

BooksTabConfigWidget::BooksTabConfigWidget(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", BOOKS_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);
  // Price Limits Group
  QGroupBox *m_group0 = new QGroupBox(this);
  m_group0->setTitle(tr("Book price limits"));
  QGridLayout *grLayout2 = new QGridLayout(m_group0);
  grLayout2->setContentsMargins(5, 5, 5, 5);
  m_minPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_minPrice->setObjectName("book_price_lowest");
  m_minPrice->setToolTip(tr("Underlimit for book prices."));
  m_minPrice->setBuddyLabel(tr("Price underlimit"));
  grLayout2->addWidget(m_minPrice, 1, 0, 1, 1);
  m_normalPrice = new AntiquaCRM::PriceEdit(m_group0);
  m_normalPrice->setObjectName("book_price_normal");
  m_normalPrice->setToolTip(tr("Default book price when a new created."));
  m_normalPrice->setBuddyLabel(tr("Default on create"));
  m_normalPrice->appendStretch(1);
  grLayout2->addWidget(m_normalPrice, 1, 1, 1, 1);
  m_group0->setLayout(grLayout2);
  layout->addWidget(m_group0);

  layout->addStretch(1);
  setLayout(layout);
}

void BooksTabConfigWidget::loadSectionConfig() {
  QJsonDocument jsDocument;
  QString sql("SELECT cfg_jsconfig FROM antiquacrm_configs");
  sql.append(" WHERE cfg_group='");
  sql.append(BOOKS_CONFIG_POINTER);
  sql.append("';");
  AntiquaCRM::ASqlCore dbsql(this);
  QSqlQuery q = dbsql.query(sql);
  if (q.size() > 0) {
    q.next();
    jsDocument = QJsonDocument::fromJson(q.value(0).toByteArray());
  } else {
    qDebug() << Q_FUNC_INFO << dbsql.lastError();
  }

  if (jsDocument.isNull())
    return;

  QJsonObject _jsObject = jsDocument.object();
  QJsonObject::const_iterator it;
  for (it = _jsObject.begin(); it != _jsObject.end(); ++it) {
    AntiquaCRM::AbstractInput *inp =
        findChild<AntiquaCRM::AbstractInput *>(it.key());
    if (inp != nullptr)
      inp->setValue(it.value().toVariant());
  }
}

void BooksTabConfigWidget::saveSectionConfig() {
  QJsonObject _jsObject;
  QList<AntiquaCRM::AbstractInput *> _list =
      findChildren<AntiquaCRM::AbstractInput *>(QString());
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::AbstractInput *m_inp = _list.at(i);
    _jsObject.insert(m_inp->objectName(), m_inp->getValue().toJsonValue());
  }

  QJsonDocument jsDocument(_jsObject);
  QString _sql("DELETE FROM antiquacrm_configs WHERE");
  _sql.append(" cfg_group='" + BOOKS_CONFIG_POINTER + "';\n");
  _sql.append("INSERT INTO antiquacrm_configs (cfg_group,cfg_jsconfig)");
  _sql.append("VALUES ('" + BOOKS_CONFIG_POINTER + "','");
  _sql.append(jsDocument.toJson(QJsonDocument::Compact));
  _sql.append("');");

  AntiquaCRM::ASqlCore dbsql(this);
  dbsql.query(_sql);
  if (dbsql.lastError().isEmpty())
    return;

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << dbsql.lastError();
#endif
}

AntiquaCRM::TabsConfigWidget::ConfigType BooksTabConfigWidget::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QIcon BooksTabConfigWidget::getIcon() const {
  return AntiquaCRM::AntiquaApplIcon("view-log");
}

const QString BooksTabConfigWidget::getTitle() const { return tr("Books"); }
