// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configlookandfeel.h"
#include "awhatsthisbutton.h"

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
    : AntiquaCRM::TabsConfigWidget{"General", "window_behavior", parent} {
  setWindowTitle(getMenuEntry().value("title").toString());

  // Central Widget for Scrollarea
  QWidget *m_central = new QWidget(this);
  m_central->setContentsMargins(0, 0, 0, 20);

  QString info;
  int _row = 0;
  layout = new LookAndFeelLayout(m_central);

  info = tr("All settings in this area require a restart of the application.");
  layout->addWidget(new QLabel(info, m_central), _row++, 0, 1, 1);

  info = tr("Show tooltip symbols after input fields.");
  m_toolTipIcons = new QCheckBox(info, m_central);
  m_toolTipIcons->setObjectName("display_tooltip_buttons");
  layout->addWidget(m_toolTipIcons, _row, 0, 1, 1);

  info = tr("If enabled, it will append to all supported input fields a "
            "clickable icon with additional popup information.");
  layout->addToolTip(_row++, 1, info);

  info = tr("Enable mouse wheel support for input fields and register tabs.");
  m_wheelSupport = new QCheckBox(info, m_central);
  m_wheelSupport->setObjectName("mouse_wheel_support");
  layout->addWidget(m_wheelSupport, _row, 0, 1, 1);

  info = tr("If switched off, unintentional changing of entries with scrolling "
            "is prevented.<br>Experience has shown that older people in "
            "particular have problems with large input masks.");
  layout->addToolTip(_row++, 1, info);

  layout->setRowStretch(_row, 1);
  m_central->setLayout(layout);
  setWidget(m_central);
}

void ConfigLookAndFeel::loadSectionConfig() {
  config->beginGroup("window_behavior");
  m_toolTipIcons->setChecked(
      config->value("display_tooltip_buttons", true).toBool());
  m_wheelSupport->setChecked(
      config->value("mouse_wheel_support", false).toBool());
  config->endGroup();
}

void ConfigLookAndFeel::saveSectionConfig() {
  config->beginGroup("window_behavior");
  config->setValue("display_tooltip_buttons", m_toolTipIcons->isChecked());
  config->setValue("mouse_wheel_support", m_wheelSupport->isChecked());
  config->endGroup();
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigLookAndFeel::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ConfigLookAndFeel::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "preferences-desktop");
  _o.insert("title", tr("Look & Feel"));
  _o.insert("tooltip", tr("Configure behavior, Look & Feel."));
  return _o;
}
