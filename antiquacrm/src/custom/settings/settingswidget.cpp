#include "settingswidget.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget{parent}
    , p_configSection(QString())
{
    setObjectName("settingswidget");
}

const QString SettingsWidget::configSection()
{
    return p_configSection;
}

void SettingsWidget::setConfigSection(const QString &str)
{
    p_configSection = str;
    emit configSectionChanged(p_configSection);
}
