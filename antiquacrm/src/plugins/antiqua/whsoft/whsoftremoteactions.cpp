// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoftremoteactions.h"
#include "whsoftjsonquery.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QLayout>

static const QIcon btnIcon() {
  QDir p = ApplSettings::getDataTarget("icons");
  QFileInfo fileInfo(p, "info.png");
  if (fileInfo.isReadable()) {
    QImageReader reader(fileInfo.filePath());
    QImage img = reader.read();
    if (!img.isNull())
      return QIcon(QPixmap::fromImage(img));
  }
  return QIcon();
}

/**
 * @brief BF_StartPage
 */
BF_StartPage::BF_StartPage(QWidget *parent) : QWidget{parent} {
  setObjectName("whsoft_action_start_page");
  setMinimumSize(QSize(300, 200));
  QHBoxLayout *layout = new QHBoxLayout(this);

  QIcon icon = btnIcon();
  QSize iconSize(24, 24);

  btn_status = new QPushButton(icon, tr("Status"), this);
  btn_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_status->setIconSize(iconSize);
  QFont btn_font = btn_status->font();
  btn_font.setPointSize((btn_font.pointSize() + 2));
  btn_status->setFont(btn_font);
  // Bestellstatus aktualisieren.
  btn_status->setToolTip(tr("Order Status update."));
  layout->addWidget(btn_status);

  btn_cancel = new QPushButton(icon, tr("Cancel"), this);
  btn_cancel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  btn_cancel->setFont(btn_font);
  btn_cancel->setIconSize(iconSize);
  // Stornieren einer kompletten Bestellung.
  btn_cancel->setToolTip(tr("Cancel an entire order."));
  layout->addWidget(btn_cancel);

  setLayout(layout);

  connect(btn_status, SIGNAL(clicked()), this, SLOT(statusClicked()));
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void BF_StartPage::statusClicked() { emit sendGotoPage(1); }

void BF_StartPage::cancelClicked() { emit sendGotoPage(2); }

/**
 * @brief WHSoftRemoteActions
 */
WHSoftRemoteActions::WHSoftRemoteActions(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setWindowTitle("Buchfreund [*]");
  setContentsMargins(5, 0, 5, 10);

  m_requester = new WHSoftJSonQuery(this);
  m_requester->setObjectName("bf_remote_actions");

  // BEGIN MainLayout
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);
  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);
  layout->setStretch(0, 1);
  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_buttonBar);
  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);
  // END

  // BEGIN Pages
  int page = 0;
  m_startPage = new BF_StartPage(m_stackedWidget);
  m_stackedWidget->insertWidget(page++, m_startPage);

  // END

  // SIGNALS
  // connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
  // SLOT(setCurrentIndex(int)));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void WHSoftRemoteActions::prepareAction(const QJsonObject &jsObj) {
  QString action = jsObj.value("action").toString();
  QString type = jsObj.value("type").toString();
  QString value = jsObj.value("value").toString();
  qDebug() << Q_FUNC_INFO << action << type << value;
}

void WHSoftRemoteActions::pushMessage(const QString &msg) {
  m_statusBar->showMessage(msg, (1000 * 6));
}

void WHSoftRemoteActions::jsonResponse(const QJsonDocument &jdoc) {
  if (QJsonValue(jdoc["status"]).toString() != "OK") {
    QString value = QJsonValue(jdoc["returnValue"]).toString();
    qWarning("WHSoft Error: %s", qPrintable(value));
    return;
  }
  QString value = QJsonValue(jdoc["returnValue"]).toString();
  pushMessage(value.trimmed());
}

void WHSoftRemoteActions::setPurchaser(const QString &person) {
  if (person.isEmpty())
    return;

  p_purchaser = person;
}

void WHSoftRemoteActions::setEMail(const QString &email) {
  if (email.isEmpty())
    return;

  p_purchaser_mail = email;
}

int WHSoftRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
