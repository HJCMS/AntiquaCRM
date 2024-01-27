// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetable.h"
#include "antiquaicon.h"

#include <AntiquaPrinting>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>

PurchaseTable::PurchaseTable(QWidget *parent) : QTableWidget{parent} {
  setColumnCount(6);
  addHeaderItem(0, tr("Provider"));
  addHeaderItem(1, tr("Article"));
  addHeaderItem(2, tr("Amount"));
  addHeaderItem(3, tr("Price"));
  addHeaderItem(4, tr("Type"));
  addHeaderItem(5, tr("Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

void PurchaseTable::contextMenuEvent(QContextMenuEvent *e) {
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_find = m->addAction(AntiquaCRM::antiquaIcon("view-search"),
                                  tr("check article duration"));
  connect(ac_find, SIGNAL(triggered()), SIGNAL(sendCheckArticles()));

  QAction *ac_copy =
      m->addAction(AntiquaCRM::antiquaIcon("edit-copy"), tr("copy article id"));
  connect(ac_copy, SIGNAL(triggered()), SLOT(copyIdToClipboard()));

  QAction *ac_open = m->addAction(AntiquaCRM::antiquaIcon("action-add"),
                                  tr("open article id"));
  connect(ac_open, SIGNAL(triggered()), SLOT(prepareOpenArticle()));

  QAction *ac_print = m->addAction(AntiquaCRM::antiquaIcon("printer"),
                                   tr("open card printing"));
  connect(ac_print, SIGNAL(triggered()), SLOT(createPrintingCard()));

  m->exec(e->globalPos());
  delete m;
}

void PurchaseTable::prepareOpenArticle() {
  qint64 id = getSelectedArticleId();
  if (id > 0)
    emit sendOpenArticle(id);
}

void PurchaseTable::copyIdToClipboard() {
  QString buf = item(currentItem()->row(), 1)->text();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void PurchaseTable::createPrintingCard() {
  qint64 aid = getSelectedArticleId();
  if (aid < 1)
    return;

  AntiquaCRM::ASqlFiles _tpl("query_printing_cards");
  if (!_tpl.openTemplate())
    return;

  _tpl.setWhereClause("i_id=" + QString::number(aid));
  AntiquaCRM::ASqlCore _sql(this);
  QHash<QString, QVariant> _data;
  QSqlQuery _q = _sql.query(_tpl.getQueryContent());
  if (_q.size() > 0) {
    _q.next();
    QSqlRecord _record = _q.record();
    for (int c = 0; c < _record.count(); c++) {
      QSqlField _field = _record.field(c);
      _data.insert(_field.name(), _field.value());
      // qDebug() << _field.name() << _field.value();
    }
  }
  _q.clear();

  if (_data.size() < 2)
    return;

  QJsonObject _options;
  _options.insert("aid", aid);
  _options.insert("author", _data.value("author").toString());
  _options.insert("title", _data.value("title").toString());
  _options.insert("year", _data.value("year").toString());
  _options.insert("changed", _data.value("since").toString());
  _options.insert("compartment", _data.value("keywords").toString());
  _options.insert("storage", _data.value("storage_id").toInt());
  _options.insert("name", _data.value("storage_name").toString());
  _options.insert("category", _data.value("storage_category").toString());
  _options.insert("basename", AntiquaCRM::AUtil::zerofill(aid));

  QUrl _qr_url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("printer");
  _qr_url.setUrl(cfg.value("qrcode_url").toString());
  QString _query(cfg.value("qrcode_query").toString());
  _query.append("=");
  _query.append(QString::number(aid));
  _qr_url.setQuery(_query);
  _options.insert("qrquery", _qr_url.toString());
  cfg.endGroup();

  AntiquaCRM::PrintBookCard *d = new AntiquaCRM::PrintBookCard(this);
  d->exec(_options, false);
}

void PurchaseTable::addHeaderItem(int i, const QString &name) {
  QTableWidgetItem *item = new QTableWidgetItem(name, QTableWidgetItem::Type);
  setHorizontalHeaderItem(i, item);
}

void PurchaseTable::setArticleStatus(const QString &article, bool available) {
  for (int r = 0; r < rowCount(); r++) {
    QTableWidgetItem *cellItem = item(r, 1);
    if (cellItem == nullptr || cellItem->text() != article)
      continue;

    if (available) {
      cellItem->setData(Qt::DecorationRole,
                        AntiquaCRM::antiquaIcon("dialog-ok-apply"));
    } else {
      cellItem->setData(Qt::DecorationRole,
                        AntiquaCRM::antiquaIcon("edit-delete"));
      // reset quantity
      item(r, 2)->setData(Qt::EditRole, 0);
    }
  }
}

qint64 PurchaseTable::getSelectedArticleId() {
  QString buf = item(currentItem()->row(), 1)->text();
  bool b = true;
  int id = buf.toInt(&b);
  return (b && id > 0) ? id : 0;
}

QTableWidgetItem *PurchaseTable::createItem(const QJsonValue &data) const {
  QString _title;
  if (data.type() == QJsonValue::Double) {
    qint64 i = data.toInt();
    _title = QString::number(i);
  } else if (data.type() == QJsonValue::Bool) {
    _title = (data.toBool()) ? tr("Yes") : tr("No");
  } else {
    _title = data.toString().trimmed();
  }

  QTableWidgetItem *item = new QTableWidgetItem(_title);
  item->setData(Qt::UserRole, data.toVariant());
  item->setFlags(Qt::ItemIsEnabled);
  return item;
}

const QStringList PurchaseTable::getArticleIds() {
  QStringList _l;
  for (int _r = 0; _r < rowCount(); _r++) {
    QTableWidgetItem *quantity = item(_r, 2);
    if (quantity->data(Qt::DisplayRole).toInt() < 1) {
      qInfo("Ignore row %d", _r);
      continue;
    }

    QTableWidgetItem *m = item(_r, 1);
    if (m != nullptr) {
      _l.append(m->data(Qt::DisplayRole).toString());
    }
  }
  return _l;
}
