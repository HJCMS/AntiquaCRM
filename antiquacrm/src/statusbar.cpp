#include "statusbar.h"
#include "sqlcore.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

StatusBar::StatusBar(QWidget *parent) : QStatusBar{parent} {
  setObjectName("StatusBar");

  m_sql = new HJCMS::SqlCore(this);

  QWidget *m_widget = new QWidget(this);
  m_widget->setObjectName("PostgreSQLConnectionStatus");

  QHBoxLayout *layout = new QHBoxLayout(m_widget);

  QLabel *m_dbInfo = new QLabel(this);
  m_dbInfo->setObjectName("ConnectionInfo");
  m_dbInfo->setText(tr("Database status"));
  layout->addWidget(m_dbInfo);

  m_dbStatus = new QToolButton(m_widget);
  m_dbStatus->setIcon(myIcon("db_status"));
  layout->addWidget(m_dbStatus);

  m_widget->setLayout(layout);

  addPermanentWidget(m_widget, 0);

  connect(m_sql, SIGNAL(s_noticeMessage(const QString &)), this,
          SLOT(sqlStatusMessage(const QString &)));
  connect(m_sql, SIGNAL(s_errorMessage(const QString &)), this,
          SLOT(sqlStatusMessage(const QString &)));
  connect(m_sql, SIGNAL(s_connectionStatus(bool)), this,
          SLOT(setDatabaseStatusIcon(bool)));

  // first shot
  m_sql->openDatabase();
}

void StatusBar::sqlStatusMessage(const QString &str) {
  // qDebug() << Q_FUNC_INFO << str;
  showMessage(str);
}

void StatusBar::setDatabaseStatusIcon(bool b) {
  // qDebug() << Q_FUNC_INFO << Qt::endl;
  if (b) {
    m_dbStatus->setIcon(myIcon("database"));
  } else {
    m_dbStatus->setIcon(myIcon("db_status"));
  }
}

StatusBar::~StatusBar() {}
