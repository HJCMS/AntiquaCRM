// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef VIEW_SETTINGS_H
#define VIEW_SETTINGS_H

#include "settingswidget.h"
#include <Utils>

#include <QObject>
#include <QLabel>
#include <QWidget>

class ViewSettings final : public SettingsWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *view_font_config;

private Q_SLOTS:
  void openFontDialog();

public:
  explicit ViewSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // VIEW_SETTINGS_H
