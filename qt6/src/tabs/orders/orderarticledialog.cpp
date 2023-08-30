// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderarticledialog.h"
#include "antiquaicon.h"

#include <QLayout>

OrderArticleDialog::OrderArticleDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("Search insert Article Id"));
  setMinimumWidth(450);
  setSizeGripEnabled(true);

  QGridLayout *layout = new QGridLayout(this);
  layout->setColumnStretch(0, 1);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar, 0, 0, 1, 3);

  QLabel *infoEdit = new QLabel(tr("Article Id:"), this);
  infoEdit->setAlignment((Qt::AlignRight | Qt::AlignVCenter));
  infoEdit->setIndent(5);
  layout->addWidget(infoEdit, 1, 0, 1, 1);

  m_searchEdit = new QSpinBox(this);
  m_searchEdit->setRange(0, 99999999);
  m_searchEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  layout->addWidget(m_searchEdit, 1, 1, 1, 1);

  btn_search = new QPushButton(tr("Search"), this);
  btn_search->setIcon(AntiquaCRM::antiquaIcon("system-search"));
  layout->addWidget(btn_search, 1, 2, 1, 1);

  btn_box = new QDialogButtonBox(Qt::Horizontal, this);
  btn_box->setStandardButtons(QDialogButtonBox::Apply |
                              QDialogButtonBox::Cancel);
  btn_apply = btn_box->button(QDialogButtonBox::Apply);
  btn_apply->setEnabled(false);
  layout->addWidget(btn_box, 2, 0, 1, 3);

  setLayout(layout);

  connect(btn_search, SIGNAL(clicked()), SLOT(setSearchArticle()));
  connect(btn_box, SIGNAL(rejected()), SLOT(reject()));
  connect(btn_apply, SIGNAL(clicked()), SLOT(accept()));
}

AntiquaCRM::ArticleOrderItem
OrderArticleDialog::addItem(const QString &key, const QVariant &value) const {
  return AntiquaCRM::AProviderOrder::createItem(key, value);
}

bool OrderArticleDialog::queryArticle(qint64 aid) {
  // Reset
  articleInfo.clear();
  articleId = -1;

  AntiquaCRM::ASqlFiles sqlFile("query_article_order_with_id");
  if (sqlFile.openTemplate()) {
    sqlFile.setWhereClause("i_id=" + QString::number(aid));
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
    if (q.size() > 0) {
      q.next();
      QSqlRecord r = q.record();
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        if (f.name() == "a_article_id") {
          articleId = q.value("a_article_id").toInt();
        }
        if (f.name() == "a_title") {
          articleInfo.append(QString::number(aid) + " : ");
          articleInfo.append(q.value("a_title").toString());
        }
      }
    }
  }
  return (articleId > 0);
}

void OrderArticleDialog::setSearchArticle() {
  qint64 id = m_searchEdit->value();
  if (id < 1) {
    setInfoMessage(tr("Empty entries are cowardly refused!"));
    return;
  }
  if (queryArticle(id)) {
    setInfoMessage(articleInfo);
    btn_apply->setEnabled(true);
    return;
  }
  setInfoMessage(tr("No result!"));
  btn_apply->setEnabled(false);
}

void OrderArticleDialog::clearSearchInput() { m_searchEdit->setValue(0); }

void OrderArticleDialog::setInfoMessage(const QString &message) {
  m_statusBar->showMessage(message, (60 * 1000));
}

qint64 OrderArticleDialog::getArticle() { return articleId; }

int OrderArticleDialog::exec() {
  clearSearchInput();
  m_searchEdit->setFocus();
  return QDialog::exec();
}
