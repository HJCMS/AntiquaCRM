/** @COPYRIGHT_HOLDER@ */

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QVariant>

#include <QSpacerItem>
#include <QWidget>

#include "applsettings.h"

class SettingsWidget : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")
  Q_PROPERTY(QString Section READ getSection WRITE setSection NOTIFY
                 sectionChanged REQUIRED)

private:
  QString Section;

Q_SIGNALS:
  void sectionChanged(const QString &);

protected:
  ApplSettings *config;
  Qt::Alignment grid_label_align =
      (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

  const QString getDirectory(const QString &dest);

public:
  explicit SettingsWidget(QWidget *parent = nullptr);

  /**
   * @brief loadSectionConfig
   */
  virtual void loadSectionConfig() = 0;

  /**
   * @brief saveSectionConfig
   */
  virtual void saveSectionConfig() = 0;

  /**
   * @brief setConfigSection
   */
  virtual void setSection(const QString &);

  /**
   * @brief configSection
   */
  const QString getSection();

};

#endif // SETTINGSWIDGET_H
