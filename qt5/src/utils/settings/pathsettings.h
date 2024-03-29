// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PATH_SETTINGS_H
#define PATH_SETTINGS_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QToolButton>
#include <QWidget>
#include <SettingsDialog>

/**
 * @brief The Path Settings widget
 * @ingroup _settings
 */
class PathSettings final : public SettingsWidget {
  Q_OBJECT

private:
  QGridLayout *m_layout;
  Qt::FindChildOptions p_options = Qt::FindDirectChildrenOnly;
  QLineEdit *m_images_path;   /**< Bilderarchiv */
  QLineEdit *m_delivery_path; /**< Lieferscheine */
  QLineEdit *m_invoice_path;  /**< Rechnungen */
  QLineEdit *m_reports_path;  /**< Berichte */
  QLineEdit *m_reminder_path; /**< Erinnerungen */
  QLineEdit *m_bookcard_path; /**< Buchkarten */
  QLineEdit *m_import_path;   /**< Standard Importieren pfad */
  QLabel *infoCell(const QString &title);
  QToolButton *setPathBtn(const QString &txName);

  void initSignalChanged(){/* unused */};

private Q_SLOTS:
  void openDirsDialog();

public:
  explicit PathSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // PATH_SETTINGS_H
