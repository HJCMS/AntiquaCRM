// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GENERAL_SETTINGS_H
#define GENERAL_SETTINGS_H

#include "settingswidget.h"

#include <QObject>

class GeneralSettings final : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  LineEdit *m_editImageSrc;  /**< Pfad zu den Bildquellen */
  IntSpinBox *m_searchStart; /**< Ab wann startet die Suche */
  IntSpinBox *m_minPrice;    /**< Kleinster Verkaufspreis */

private Q_SLOTS:
  void setImageDir();

public:
  explicit GeneralSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // GENERAL_SETTINGS_H
