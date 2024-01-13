// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configstoragecache.h"

#include <QLabel>

ConfigStorageCache::ConfigStorageCache(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"cacheconf", parent} {
  setWindowTitle(tr("Cache config"));
  setObjectName("antiquacrm_cacheconf");
  setContentsMargins(5, 5, 5, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QStringList _labels({"Id",                         // item id
                       tr("Table name"),             // tabele name
                       tr("File basename"),          // file basename
                       tr("Title"),                  // status display titel
                       tr("Days until to update")}); // expiration days

  m_table = new QTableWidget(this);
  m_table->setColumnCount(_labels.size());
  m_table->setHorizontalHeaderLabels(_labels);
  m_header = m_table->horizontalHeader();
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);

  layout->addWidget(m_table);
  layout->setStretch(0, 1);

  QString _info =
      tr("In order to reduce the network load in remote office AntiquaCRM "
         "works with a hard drive storage. You can set the cycle with which "
         "the memory is updated at Application start.");

  QLabel *m_lb = new QLabel(_info, this);
  m_lb->setWordWrap(true);
  layout->addWidget(m_lb);
  setLayout(layout);

  connect(m_table, SIGNAL(itemChanged(QTableWidgetItem *)),
          SLOT(columnChanged(QTableWidgetItem *)));
}

void ConfigStorageCache::columnChanged(QTableWidgetItem *) {
  setWindowModified(true);
}

void ConfigStorageCache::loadSectionConfig() {
  if (pgsql == nullptr)
    pgsql = new AntiquaCRM::ASqlCore(this);

  QString _sql("SELECT * FROM antiquacrm_cacheconf ORDER BY cache_id;");
  QSqlQuery _q = pgsql->query(_sql);
  if (_q.size() < 1)
    return;

  int _row = 0;
  const QStringList _editable({"cache_display", "cache_runtime"});
  while (_q.next()) {
    m_table->setRowCount(_q.size());
    QSqlRecord _record = _q.record();
    for (int c = 0; c < _record.count(); c++) {
      const QSqlField _field = _record.field(c);
      QTableWidgetItem *item = new QTableWidgetItem(QTableWidgetItem::UserType);
      item->setData(Qt::EditRole, _field.value());
      if (!_editable.contains(_field.name())) {
        item->setFlags(Qt::NoItemFlags);
      }
      m_table->setItem(_row, c, item);
    }
    _row++;
  }
}

void ConfigStorageCache::saveSectionConfig() {
  if (pgsql == nullptr)
    pgsql = new AntiquaCRM::ASqlCore(this);

  QStringList _queries;
  const QSqlRecord _record = pgsql->record("antiquacrm_cacheconf");
  for (int r = 0; r < m_table->rowCount(); r++) {
    QString _sql("UPDATE antiquacrm_cacheconf SET ");
    int _id = m_table->item(r, 0)->data(Qt::EditRole).toInt();
    QStringList _columnData;
    for (int c = 1; c < m_table->columnCount(); c++) {
      QSqlField _field = _record.field(c);
      QTableWidgetItem *item = m_table->item(r, c);
      if (item->flags() & Qt::ItemIsEditable) {
        QVariant _v = item->data(Qt::EditRole);
        if (_v.metaType().id() == QMetaType::QString)
          _columnData.append(_field.name() + "='" + _v.toString() + "'");
        else
          _columnData.append(_field.name() + "=" + _v.toString());
      }
    }
    _sql.append(_columnData.join(","));
    _sql.append(" WHERE cache_id=" + QString::number(_id) + ";");
    _queries << _sql;
  }

  if (_queries.size() > 0) {
    pgsql->query(_queries.join("\n"));
    if (pgsql->lastError().isEmpty())
      setWindowModified(false);

    _queries.clear();
  }
}

AntiquaCRM::ConfigType ConfigStorageCache::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject ConfigStorageCache::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "network-server-database");
  _o.insert("title", tr("Disk storage"));
  _o.insert("tooltip", tr("Storage cache configuration."));
  return _o;
}
