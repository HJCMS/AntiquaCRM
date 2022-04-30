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

class GeneralSettingsWidget : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QLineEdit *m_editImageSrc; /**< Pfad zu den Bildquellen */
  QSpinBox *m_searchStart; /**< Ab wann startet die Suche */
  QHash<QString, QVariant> p_hash;

public Q_SLOTS:
  void updateConfigSets(const QHash<QString, QVariant> &);

public:
  explicit GeneralSettingsWidget(QWidget *parent = nullptr);
  const QHash<QString, QVariant> &getSectionConfig();
};

#endif // GENERALSETTINGSWIDGET_H
