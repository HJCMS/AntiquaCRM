/** @COPYRIGHT_HOLDER@ */

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>

class SettingsWidget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )
    Q_PROPERTY ( QString ConfigSection READ configSection WRITE setConfigSection NOTIFY configSectionChanged REQUIRED )

private:
    QString p_configSection;

Q_SIGNALS:
    void configSectionChanged(const QString &);

public Q_SLOTS:
    virtual void updateConfigSets(const QHash<QString,QVariant> &) = 0;

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    const QString configSection();
    void setConfigSection(const QString &);
    virtual const QHash<QString,QVariant> & getSectionConfig() = 0;
};

#endif // SETTINGSWIDGET_H
