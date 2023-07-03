// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configtabsview.h"

#include <QLabel>
#include <QLayout>

ConfigTabsView::ConfigTabsView(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", "config_tabs", parent} {
  setWindowTitle(tr("Tabs"));
  setObjectName("config_tabs");
  setContentsMargins(5, 5, 5, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  // layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(new QLabel("__TODO__", this));
  layout->addStretch(1);
  setLayout(layout);
}

void ConfigTabsView::loadSectionConfig() { qDebug() << Q_FUNC_INFO << "TODO"; }

void ConfigTabsView::saveSectionConfig() { qDebug() << Q_FUNC_INFO << "TODO"; }

AntiquaCRM::TabsConfigWidget::ConfigType ConfigTabsView::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

QList<AntiquaCRM::TabsInterface *> ConfigTabsView::viewableTabs() {
  AntiquaCRM::TabsLoader _loader(this);
  QList<AntiquaCRM::TabsInterface *> _list = _loader.interfaces(this);
  /*
   * TODO TABS Configuration
  QList<QJsonObject> _jsl;
  for (int i = 0; i < _list.size(); i++) {
    AntiquaCRM::TabsInterface *m_tab = _list.at(i);
    if (m_tab != nullptr)
      _jsl.append(m_tab->menuEntry());
  }
  */
  return _list;
}

const QJsonObject ConfigTabsView::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "tab-detach");
  _o.insert("title", tr("Tabs visibility"));
  _o.insert("tooltip", tr("Configure accessibility of tabs."));
  return _o;
}
