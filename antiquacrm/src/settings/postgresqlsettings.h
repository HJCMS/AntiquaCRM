/** @COPYRIGHT_HOLDER@ */

#ifndef POSTGRESQLSETTINGS_H
#define POSTGRESQLSETTINGS_H

#include "settingswidget.h"

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

class PostgreSqlSettings : public SettingsWidget
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    QLineEdit *psql_hostname;
    QLineEdit *psql_databasename;
    QLineEdit *psql_username;
    QLineEdit *psql_password;
    QSpinBox  *psql_port;
    QGroupBox *psql_ssl;
    QLineEdit *psql_ssl_clientcert;
    QHash<QString,QVariant> p_hash;

public Q_SLOTS:
    void updateConfigSets(const QHash<QString,QVariant> &);

public:
    explicit PostgreSqlSettings(QWidget *parent = nullptr);
    const QHash<QString,QVariant> & getSectionConfig();

};

#endif // POSTGRESQLSETTINGS_H
