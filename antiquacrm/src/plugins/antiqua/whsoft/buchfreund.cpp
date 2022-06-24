// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
#include "whsoftconfig.h"
#include "whsoftjsonquery.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

Buchfreund::Buchfreund(const QString &widgetId, QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("buchfreund_main");
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Artikel Bestandsänderungen
  QHBoxLayout *r0Layout = new QHBoxLayout();
  QGroupBox *m_duration = new QGroupBox(this);
  m_duration->setTitle(tr("Modify Item inventory count"));
  QGridLayout *durLayout = new QGridLayout(m_duration);
  QLabel *lb1 = new QLabel(tr("Article Id") + ":", m_duration);
  durLayout->addWidget(lb1, 0, 0, 1, 1, Qt::AlignRight);
  m_articleId = new QLineEdit(m_duration);
  durLayout->addWidget(m_articleId, 0, 1, 1, 1, Qt::AlignLeft);
  m_count = new QSpinBox(m_duration);
  m_count->setToolTip(tr("itemcount"));
  m_count->setRange(0, 99);
  durLayout->addWidget(m_count, 1, 0, 1, 1, Qt::AlignRight);
  QPushButton *btn_queryArticle = new QPushButton(m_duration);
  btn_queryArticle->setText(tr("Update"));
  durLayout->addWidget(btn_queryArticle, 1, 1, 1, 1, Qt::AlignLeft);
  m_duration->setLayout(durLayout);
  r0Layout->addWidget(m_duration);
  r0Layout->addStretch(1);
  layout->addLayout(r0Layout);

  layout->addStretch(1);
  QLabel *lb2 = new QLabel(tr("Response Information"), this);
  layout->addWidget(lb2);

  m_response = new QTextEdit(this);
  layout->addWidget(m_response);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_queryArticle, SIGNAL(clicked()), this,
          SLOT(updateArticleCount()));
}

void Buchfreund::jsonQuery(const QString &operation, const QJsonDocument &doc) {
  WHSoftJSonQuery *mq = new WHSoftJSonQuery(this);
  mq->setObjectName("json_query_buchfreund_counts");
  connect(mq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(queryResponse(const QJsonDocument &)));

  mq->customQuery(operation, doc);
}

void Buchfreund::updateArticleCount() {
  int id = m_articleId->text().toInt();
  if (id < 1)
    return;

  int count = m_count->value();
  QJsonDocument doc = createUpdateArtcileCount(id, count);
  jsonQuery("bestand", doc);
}

void Buchfreund::queryResponse(const QJsonDocument &doc) {
  QString d = QString::fromLocal8Bit(doc.toJson(QJsonDocument::Indented));
  m_response->setPlainText(d);
}
