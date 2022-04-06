#include "statusbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

StatusBar::StatusBar(QWidget *parent) : QStatusBar{parent} {
  setObjectName("StatusBar");

  createConnectionWidget();
  addPermanentWidget(m_connectionWidget, 0);
}

void StatusBar::createConnectionWidget() {
  m_connectionWidget = new QWidget(this);
  m_connectionWidget->setObjectName("PostgreSQLConnectionStatus");

  QHBoxLayout *layout = new QHBoxLayout(m_connectionWidget);

  QLabel *m_dbInfo = new QLabel(this);
  m_dbInfo->setObjectName("ConnectionInfo");
  m_dbInfo->setText(tr("Database status"));
  layout->addWidget(m_dbInfo);

  m_dbStatusButton = new QToolButton(m_connectionWidget);
  m_dbStatusButton->setIcon(myIcon("db_status"));
  layout->addWidget(m_dbStatusButton);

  m_connectionWidget->setLayout(layout);
}

void StatusBar::sqlStatusMessage(const QString &str) {
  // qDebug() << "StatusBar::sqlStatusMessage: " << str;
  showMessage(str);
}

void StatusBar::setDatabaseStatusIcon(bool b) {
  if (b) {
    m_dbStatusButton->setIcon(myIcon("database"));
  } else {
    m_dbStatusButton->setIcon(myIcon("db_status"));
  }
}

StatusBar::~StatusBar() {}
