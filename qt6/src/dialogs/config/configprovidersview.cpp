// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configprovidersview.h"
#include "pluginlistwidget.h"

#include <QLabel>
#include <QLayout>

ConfigProvidersView::ConfigProvidersView(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"config_providers", parent} {
  setWindowTitle(tr("Providers"));
  setObjectName("config_providers");
  setContentsMargins(5, 5, 5, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_view = new PluginListWidget(this);
  layout->addWidget(m_view);
  layout->setStretch(0, 1);

  setLayout(layout);
}

void ConfigProvidersView::loadSectionConfig() {
  AntiquaCRM::ProvidersLoader _loader(this);
  foreach (QString path, _loader.getInterfaceList()) {
    if (!_loader.setInterfaceName(path))
      continue;

    QJsonObject _jso = _loader.metaData().value("MetaData").toObject();
    if (_jso.isEmpty())
      continue;

    m_view->addListItem(_jso);
  }

  QMap<QString, bool> _vmap;
  config->beginGroup("plugin/providers/enable");
  foreach (QString k, config->childKeys()) {
    _vmap.insert(k, config->value(k).toBool());
  }
  config->endGroup();
  m_view->setStatus(_vmap);
}

void ConfigProvidersView::saveSectionConfig() {
  QMapIterator<QString, bool> _status(m_view->getStatus());
  config->beginGroup("plugin/providers/enable");
  while (_status.hasNext()) {
    _status.next();
    config->setValue(_status.key(), _status.value());
  }
  config->endGroup();
}

AntiquaCRM::ConfigType
ConfigProvidersView::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject ConfigProvidersView::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "tab-detach");
  _o.insert("title", tr("Providers"));
  _o.insert("tooltip", tr("Configure Providers."));
  return _o;
}
