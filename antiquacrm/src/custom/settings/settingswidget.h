/** @COPYRIGHT_HOLDER@ */

#ifndef WIDGET_SETTINGS_H
#define WIDGET_SETTINGS_H

#include <QObject>
#include <QSpacerItem>
#include <QString>
#include <QWidget>
#include <QIcon>

#include <Utils>

class ApplSettings;

/**
 * @brief Definitionsklasse für den Konfigurations Dialog
 * @group Configuration Widgets
 * @class SettingsWidget
 * @abstract
 */
class SettingsWidget : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")
  Q_PROPERTY(QString pageTitle READ getPageTitle WRITE setPageTitle NOTIFY pageTitleChanged REQUIRED)
  Q_PROPERTY(QIcon pageIcon READ getPageIcon WRITE setPageIcon NOTIFY pageIconChanged REQUIRED)

protected:
  QString pageTitle;
  QIcon pageIcon;

  /**
   * @brief Globale Settings instanz
   */
  ApplSettings *config;

  /**
   * @brief Layout ausrichtung innerhalb der Kategorien
   */
  Qt::Alignment grid_label_align =
      (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

  /**
   * @brief Verzeichnis Dialog
   */
  const QString getDirectory(const QString &dest);

Q_SIGNALS:
  void pageTitleChanged();
  void pageIconChanged();

public:
  explicit SettingsWidget(QWidget *parent = nullptr);

  /**
   * @brief Hier wird der Menütitel definiert!
   */
  virtual void setPageTitle(const QString &) = 0;

  /**
   * @brief Gibt den Menüeintrag zurück
   */
  virtual const QString getPageTitle() = 0;

  /**
   * @brief Hier wird der Menütitel definiert!
   */
  virtual void setPageIcon(const QIcon &) = 0;

  /**
   * @brief Gibt den Menüeintrag zurück
   */
  virtual const QIcon getPageIcon() = 0;

  /**
   * @brief Lade die Konfiguration für ...
   */
  virtual void loadSectionConfig() = 0;

  /**
   * @brief Aktuelle Einstellungen speichern.
   */
  virtual void saveSectionConfig() = 0;
};

#endif // WIDGET_SETTINGS_H
