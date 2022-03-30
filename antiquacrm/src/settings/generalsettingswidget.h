/** @COPYRIGHT_HOLDER@ */

#ifndef GENERALSETTINGSWIDGET_H
#define GENERALSETTINGSWIDGET_H

#include "settingswidget.h"

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

class GeneralSettingsWidget : public SettingsWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    QHash<QString,QVariant> p_hash;

public Q_SLOTS:
    void updateConfigSets(const QHash<QString,QVariant> &);

public:
    explicit GeneralSettingsWidget(QWidget *parent = nullptr);
    const QHash<QString,QVariant> & getSectionConfig();
};

#endif // GENERALSETTINGSWIDGET_H
