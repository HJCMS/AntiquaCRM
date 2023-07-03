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
  setContentsMargins(5, 5, 5, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  m_view = new PluginListWidget(this);
  layout->addWidget(m_view);
  layout->setStretch(0, 1);
  setLayout(layout);
}

void ConfigTabsView::loadSectionConfig() { qDebug() << Q_FUNC_INFO << "TODO"; }

void ConfigTabsView::saveSectionConfig() { qDebug() << Q_FUNC_INFO << "TODO"; }

AntiquaCRM::TabsConfigWidget::ConfigType ConfigTabsView::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

QList<AntiquaCRM::TabsInterface *> ConfigTabsView::viewableTabs() {
  AntiquaCRM::TabsLoader _loader(this);
  foreach (QString path, _loader.getInterfaceList()) {
    if (!_loader.setInterfaceName(path))
      continue;

    QJsonObject _jso = _loader.metaData().value("MetaData").toObject();
    if (_jso.isEmpty())
      continue;

    m_view->insertPlugin(_jso);
  }
  return _loader.interfaces(this);
}

const QJsonObject ConfigTabsView::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "tab-detach");
  _o.insert("title", tr("Tabs visibility"));
  _o.insert("tooltip", tr("Configure accessibility of tabs."));
  return _o;
}
