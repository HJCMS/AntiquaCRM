// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configtabsview.h"
#include "pluginlistwidget.h"

#include <QLabel>
#include <QLayout>

ConfigTabsView::ConfigTabsView(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", "config_tabs", parent} {
  setWindowTitle(tr("Tabs"));
  setObjectName("config_tabs");
  setContentsMargins(0, 0, 0, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  m_view = new PluginListWidget(this);
  layout->addWidget(m_view);
  layout->setStretch(0, 1);

  QString _info =
      tr("You can click move and drop to sort the listed plugins here.\n"
         "In addition, a selection box is used to enable and disable loading.\n"
         "Hint: This changes require a restart.");

  QLabel *m_lb = new QLabel(_info, this);
  m_lb->setIndent(5);
  m_lb->setWordWrap(true);
  layout->addWidget(m_lb);

  setLayout(layout);
}

void ConfigTabsView::loadSectionConfig() {
  AntiquaCRM::TabsLoader _loader(this);
  foreach (QString path, _loader.getInterfaceList()) {
    if (!_loader.setInterfaceName(path))
      continue;

    QJsonObject _jso = _loader.metaData().value("MetaData").toObject();
    if (_jso.isEmpty())
      continue;

    m_view->addListItem(_jso);
  }

  QMap<QString, bool> _vmap;
  config->beginGroup("plugin/tabs/enable");
  foreach (QString k, config->childKeys()) {
    _vmap.insert(k, config->value(k).toBool());
  }
  config->endGroup();
  m_view->setStatus(_vmap);
}

void ConfigTabsView::saveSectionConfig() {
  QMapIterator<QString, bool> _status(m_view->getStatus());
  config->beginGroup("plugin/tabs/enable");
  while (_status.hasNext()) {
    _status.next();
    config->setValue(_status.key(), _status.value());
  }
  config->endGroup();

  /*
   * Currently unused!
  QMapIterator<int, QString> _sort(m_view->getSort());
  config->beginWriteArray("plugin/tabs/sort");
  while (_sort.hasNext()) {
    _sort.next();
    config->setArrayIndex(_sort.key());
    config->setValue("id", _sort.value());
  }
  config->endArray();
  */
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigTabsView::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QJsonObject ConfigTabsView::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "tab-detach");
  _o.insert("title", tr("Tabs visibility"));
  _o.insert("tooltip", tr("Configure accessibility of tabs."));
  return _o;
}
