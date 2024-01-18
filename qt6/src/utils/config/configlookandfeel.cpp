// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configlookandfeel.h"
#include "iconthemes.h"

#include <QLabel>

ConfigLookAndFeel::ConfigLookAndFeel(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"window_behavior", parent} {
  setWindowTitle(getMenuEntry().value("title").toString());

  // Central Widget for Scrollarea
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QString _info;
  int _row = 0;
  AntiquaCRM::ConfigGridLayout *layout =
      new AntiquaCRM::ConfigGridLayout(m_central);

  _info = tr("All settings in this area require a restart of the application.");
  layout->addWidget(new QLabel(_info, m_central), _row++, 0, 1, 1);

  _info = tr("Show tooltip symbols after input fields.");
  m_toolTipIcons = new AntiquaCRM::BoolBox(m_central);
  m_toolTipIcons->setObjectName("display_tooltip_buttons");
  m_toolTipIcons->setBuddyLabel(_info);
  layout->addWidget(m_toolTipIcons, _row, 0, 1, 1);

  _info = tr("If enabled, it will append to all supported input fields a "
             "clickable icon with additional popup information.");
  layout->addToolTip(_row++, 1, _info);

  _info = tr("Enable mouse wheel support for input fields and register tabs.");
  m_wheelSupport = new AntiquaCRM::BoolBox(m_central);
  m_wheelSupport->setObjectName("mouse_wheel_support");
  m_wheelSupport->setBuddyLabel(_info);
  layout->addWidget(m_wheelSupport, _row, 0, 1, 1);

  _info =
      tr("If switched off, unintentional changing of entries with scrolling "
         "is prevented.<br>Experience has shown that older people in "
         "particular have problems with large input masks.");
  layout->addToolTip(_row++, 1, _info);

  m_iconThemes = new IconThemes(this);
  m_iconThemes->setBuddyLabel(tr("Set application icon theme"));
  m_iconThemes->appendStretch(0);
#ifdef Q_OS_WIN
  m_iconThemes->setEnabled(false);
#endif
  layout->addWidget(m_iconThemes, _row++, 0, 1, 2);

  // AntiquaCRM::TabsIndex::onEnterChange - table auto refresh
  _info = tr("From which number of columns, plugin tables no longer be updated "
             "automatically");
  m_queryAutoUpdate = new AntiquaCRM::NumEdit(this);
  m_queryAutoUpdate->setObjectName("SqlAutoUpdateCount");
  m_queryAutoUpdate->setRange(25, 1000);
  m_queryAutoUpdate->setValue(50);
  m_queryAutoUpdate->setBuddyLabel(_info);
  // m_autoTableRefresh->setToolTip
  m_queryAutoUpdate->appendStretch(0);
  layout->addWidget(m_queryAutoUpdate, _row, 0, 1, 1);
  _info = tr("This value limits the behavior of how long tab tables "
             "automatically updated when the user requests it. Simplify, this "
             "is a performance feature for remote office clients.");
  layout->addToolTip(_row++, 1, _info);

  _info = tr("Reduces query output in inventory tables to");
  m_queryLimit = new AntiquaCRM::NumEdit(this);
  m_queryLimit->setObjectName("SqlQueryLimit");
  m_queryLimit->setRange(100, 5000);
  m_queryLimit->setValue(500);
  m_queryLimit->setBuddyLabel(_info);
  m_queryLimit->appendStretch(0);
  layout->addWidget(m_queryLimit, _row, 0, 1, 1);
  _info = tr("This is a performance feature to minimize the rendering of large "
             "table outputs after sql query.");
  layout->addToolTip(_row++, 1, _info);

  layout->setRowStretch(_row, 1);
  m_central->setLayout(layout);
  setWidget(m_central);
}

void ConfigLookAndFeel::loadSectionConfig() {
  // window_behavior
  config->beginGroup("window_behavior");
  m_toolTipIcons->setValue(
      config->value("display_tooltip_buttons", true).toBool());
  m_wheelSupport->setValue(
      config->value("mouse_wheel_support", false).toBool());
  config->endGroup();
  // database
  config->beginGroup("database");
  m_queryAutoUpdate->setValue(config->value("SqlAutoUpdateCount", 50).toInt());
  m_queryLimit->setValue(config->value("SqlQueryLimit", 50).toInt());
  config->endGroup();

  // General
  m_iconThemes->setValue(
      config->value("icon_theme", QIcon::fallbackThemeName()).toString());

  registerInputChangeSignals(this);
}

void ConfigLookAndFeel::saveSectionConfig() {
  // window_behavior
  config->beginGroup("window_behavior");
  config->setValue("display_tooltip_buttons", m_toolTipIcons->getValue());
  config->setValue("mouse_wheel_support", m_wheelSupport->getValue());
  config->endGroup();
  // database
  config->beginGroup("database");
  config->setValue("SqlAutoUpdateCount", m_queryAutoUpdate->getValue());
  config->setValue("SqlQueryLimit", m_queryLimit->getValue());
  config->endGroup();

  // General
  config->setValue("icon_theme", m_iconThemes->getValue());
  setWindowModified(false);
}

AntiquaCRM::ConfigType ConfigLookAndFeel::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigLookAndFeel::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "preferences-desktop");
  _o.insert("title", tr("Look & Feel"));
  _o.insert("tooltip", tr("Configure behavior, Look & Feel."));
  return _o;
}
