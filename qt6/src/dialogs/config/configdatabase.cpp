// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdatabase.h"

#include <QLocale>
#include <QMetaType>
#include <QtWidgets>

ConfigDatabase::ConfigDatabase(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"database", QString(), parent} {
  setWindowTitle(getMenuEntry().value("title").toString());
  // Central Widget
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QVBoxLayout *layout = new QVBoxLayout(m_central);
  layout->setContentsMargins(5, 5, 5, 5);

  layout->addWidget(new QLabel(tr("Database"), this));

  m_central->setLayout(layout);
  layout->addStretch(1);
  setWidget(m_central);
}

void ConfigDatabase::loadSectionConfig() {}

void ConfigDatabase::saveSectionConfig() {}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigDatabase::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigDatabase::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "network-server-database");
  _o.insert("title", tr("Database"));
  _o.insert("tooltip", tr("Database configuration."));
  return _o;
}
