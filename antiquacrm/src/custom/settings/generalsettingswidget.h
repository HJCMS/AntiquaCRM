/** @COPYRIGHT_HOLDER@ */

#ifndef GENERALSETTINGSWIDGET_H
#define GENERALSETTINGSWIDGET_H

#include "settingswidget.h"

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

#include <Utils>

class GeneralSettingsWidget : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  LineEdit *m_editImageSrc;  /**< Pfad zu den Bildquellen */
  IntSpinBox *m_searchStart; /**< Ab wann startet die Suche */
  IntSpinBox *m_minPrice;    /**< Kleinster Verkaufspreis */

private Q_SLOTS:
  void setImageDir();

public:
  explicit GeneralSettingsWidget(QWidget *parent = nullptr);
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // GENERALSETTINGSWIDGET_H
