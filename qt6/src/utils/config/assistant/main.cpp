// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "assistant.h"
#include <AntiquaCRM>
#include <QApplication>
#include <QIcon>
#include <QStyleFactory>
#include <QTranslator>

void initialAssistantTheme(QApplication& appl) {
  Q_INIT_RESOURCE(assistant);
  appl.setStyle(QStyleFactory::create("Fusion"));

  // WARNING - Do not init Database Connections in constructors!
  AntiquaCRM::ASettings* cfg = new AntiquaCRM::ASettings(&appl);

  // Required for System Desktop changes
  const QString _platform = appl.platformName().toLower().trimmed();

  QIcon::setFallbackThemeName("hicolor");
  QPalette _palette = appl.palette();
  // @fixme KDE theme
  if (_platform.startsWith("xcb")) {
    const QColor _rgb = _palette.color(QPalette::PlaceholderText).toRgb();
    if (!AntiquaCRM::AColorLuminance(&appl).checkForeground(_rgb)) {
      _palette.setColor(QPalette::PlaceholderText, Qt::darkGray);
    }
  }
  // @fixme Windows theme
  if (_platform.startsWith("windows")) {
    QColor _highlight(255, 255, 127);
    _palette.setColor(QPalette::Inactive, QPalette::Highlight, _highlight);
  }
  appl.setPalette(_palette);

  const QDir _dir(cfg->getTranslationDir());
  if (!_dir.isReadable()) {
    qWarning("No access to %s", qPrintable(_dir.path()));
    cfg->deleteLater();
    return;
  }

  QTranslator* m_qtr = new QTranslator(&appl);
  if (m_qtr->load(QLocale::system(), "antiquacrm", "_", _dir.path(), ".qm"))
    appl.installTranslator(m_qtr);

  cfg->deleteLater();
}

int main(int argc, char* argv[]) {
  QApplication appl(argc, argv);
  appl.setApplicationName(ANTIQUACRM_NAME);
  appl.setDesktopFileName(ANTIQUACRM_NAME);
  appl.setApplicationVersion(ANTIQUACRM_VERSION);
  appl.setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  initialAssistantTheme(appl);
  Assistant assistant;
  return assistant.exec();
}
