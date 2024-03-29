// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configtreewidget.h"
#include "antiquaicon.h"

ConfigTreeWidget::ConfigTreeWidget(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(1);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);

  QTreeWidgetItem *m_header = headerItem();
  m_header->setText(0, tr("Configuration Menu"));
  m_header->setIcon(0, AntiquaCRM::antiquaIcon("antiquacrm"));

  m_mainIndex = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  m_mainIndex->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  m_mainIndex->setFlags(Qt::ItemIsEnabled);
  m_mainIndex->setIcon(0, AntiquaCRM::antiquaIcon("configure"));
  m_mainIndex->setText(0, tr("General"));
  m_mainIndex->setExpanded(true);
  addTopLevelItem(m_mainIndex);

  m_tabsIndex = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  m_tabsIndex->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  m_tabsIndex->setFlags(Qt::ItemIsEnabled);
  m_tabsIndex->setIcon(0, AntiquaCRM::antiquaIcon("tab-new"));
  m_tabsIndex->setText(0, tr("Tab - Plugins"));
  m_tabsIndex->setData(0, Qt::ToolTipRole,
                       tr("Configure accessibility of tabs."));
  m_tabsIndex->setData(1, Qt::UserRole, QString("config_tabs"));
  m_tabsIndex->setExpanded(true);
  addTopLevelItem(m_tabsIndex);

  m_providerIndex = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  m_providerIndex->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  m_providerIndex->setFlags(Qt::ItemIsEnabled);
  m_providerIndex->setIcon(0, AntiquaCRM::antiquaIcon("folder-remote"));
  m_providerIndex->setText(0, tr("Provider - Plugins"));
  m_providerIndex->setData(0, Qt::ToolTipRole,
                           tr("Configure accessibility of providers."));
  m_providerIndex->setData(1, Qt::UserRole, QString("config_providers"));
  m_providerIndex->setExpanded(true);
  addTopLevelItem(m_providerIndex);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(setItemSelected(QTreeWidgetItem *, int)));
}

QTreeWidgetItem *ConfigTreeWidget::childItem(QTreeWidgetItem *parent) const {
  QFont _font(font());
  _font.setItalic(true);

  QTreeWidgetItem *m = new QTreeWidgetItem(parent, QTreeWidgetItem::UserType);
  m->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |
              Qt::ItemNeverHasChildren);
  m->setFont(0, _font);
  return m;
}

void ConfigTreeWidget::addChild(QTreeWidgetItem *main, int page,
                                const QJsonObject &jso) {
  const QIcon _icon = AntiquaCRM::antiquaIcon(jso.value("icon").toString());
  QTreeWidgetItem *_item = childItem(main);
  _item->setText(0, jso.value("title").toString());
  _item->setData(0, Qt::DecorationRole, _icon);
  _item->setData(0, Qt::ToolTipRole, jso.value("tooltip").toString());
  _item->setData(1, Qt::UserRole, page);
  main->addChild(_item);
}

void ConfigTreeWidget::setItemSelected(QTreeWidgetItem *item, int) {
  if (item->type() != QTreeWidgetItem::UserType) {
    const QString _id = item->data(1, Qt::UserRole).toString();
    if (_id.startsWith("config_"))
      emit sendConfigGroup(_id);

    return;
  }

  if (item->flags() == Qt::NoItemFlags)
    return;

  QVariant _page = item->data(1, Qt::UserRole);
  if (_page.isNull())
    return;

  emit sendPageIndex(_page.toInt());
}

void ConfigTreeWidget::addGeneral(int page, const QJsonObject &jobj) {
  addChild(m_mainIndex, page, jobj);
}

void ConfigTreeWidget::addTabPlugin(int page, const QJsonObject &jobj) {
  addChild(m_tabsIndex, page, jobj);
}

void ConfigTreeWidget::addProviderPlugin(int page, const QJsonObject &jobj) {
  addChild(m_providerIndex, page, jobj);
}
