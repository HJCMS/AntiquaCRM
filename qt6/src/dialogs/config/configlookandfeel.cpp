// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configlookandfeel.h"
#include "awhatsthisbutton.h"
#include "iconthemes.h"

#include <QLabel>

LookAndFeelLayout::LookAndFeelLayout(QWidget *parent) : QGridLayout{parent} {
  setContentsMargins(5, 5, 5, 5);
}

void LookAndFeelLayout::addToolTip(int row, int column, const QString &text) {
  AntiquaCRM::AWhatsThisButton *m_tbn =
      new AntiquaCRM::AWhatsThisButton(text, parentWidget());
  m_tbn->setFocusPolicy(Qt::NoFocus);
  addWidget(m_tbn, row, column, 1, 1, Qt::AlignRight);
}

ConfigLookAndFeel::ConfigLookAndFeel(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"General", "window_behavior", parent} {
  setWindowTitle(getMenuEntry().value("title").toString());

  // Central Widget for Scrollarea
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QString _info;
  int _row = 0;
  LookAndFeelLayout *layout = new LookAndFeelLayout(m_central);

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
  layout->setRowStretch(_row, 1);
  m_central->setLayout(layout);
  setWidget(m_central);
}

void ConfigLookAndFeel::loadSectionConfig() {
  config->beginGroup("window_behavior");
  m_toolTipIcons->setValue(
      config->value("display_tooltip_buttons", true).toBool());
  m_wheelSupport->setValue(
      config->value("mouse_wheel_support", false).toBool());
  config->endGroup();
  m_iconThemes->setValue(
      config->value("icon_theme", QIcon::fallbackThemeName()).toString());
  registerInputChangeSignals(this);
}

void ConfigLookAndFeel::saveSectionConfig() {
  config->beginGroup("window_behavior");
  config->setValue("display_tooltip_buttons", m_toolTipIcons->getValue());
  config->setValue("mouse_wheel_support", m_wheelSupport->getValue());
  config->endGroup();
  config->setValue("icon_theme", m_iconThemes->getValue());
  setWindowModified(false);
}

AntiquaCRM::PluginConfigWidget::ConfigType ConfigLookAndFeel::getType() const {
  return AntiquaCRM::PluginConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigLookAndFeel::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "preferences-desktop");
  _o.insert("title", tr("Look & Feel"));
  _o.insert("tooltip", tr("Configure behavior, Look & Feel."));
  return _o;
}
