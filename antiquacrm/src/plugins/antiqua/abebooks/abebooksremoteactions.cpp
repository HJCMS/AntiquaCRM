// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksremoteactions.h"
#include "abebooksrequester.h"

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

ABE_StatusPage::ABE_StatusPage(QWidget *parent) : QWidget{parent} {
  setObjectName("abebooks_status_page");
  QVBoxLayout *layout = new QVBoxLayout(this);

  m_rb1 = new QRadioButton(tr("Provider Status - no disclosures"), this);
  m_rb1->setChecked(true);
  layout->addWidget(m_rb1);

  m_rb2 = new QRadioButton(tr("Direct order accepted!"), this);
  m_rb2->setObjectName("availabilityConfirmed");
  layout->addWidget(m_rb2);

  m_rb3 = new QRadioButton(tr("The item will be shipped!"), this);
  m_rb3->setObjectName("shipped");
  layout->addWidget(m_rb3);

  m_rb4 = new QRadioButton(tr("The order is rejected!"), this);
  m_rb4->setObjectName("rejected");
  layout->addWidget(m_rb4);

  m_rb5 = new QRadioButton(tr("Article is no longer available!"), this);
  m_rb5->setObjectName("previouslySold");
  layout->addWidget(m_rb5);

  m_rb6 = new QRadioButton(tr("The buyer's credit card is declined!"), this);
  m_rb6->setObjectName("creditCardDeclined");
  layout->addWidget(m_rb6);

  m_apply = new QPushButton(btnIcon(), tr("Apply"), this);
  layout->addWidget(m_apply);
  layout->addStretch(1);
  setLayout(layout);

  connect(m_apply, SIGNAL(clicked()), this, SLOT(prepareAction()));
}

void ABE_StatusPage::prepareAction() {
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        emit sendAction(rb->objectName());
        break;
      }
    }
  }
}

ABE_StartPage::ABE_StartPage(QWidget *parent) : QWidget{parent} {
  setObjectName("abebooks_action_start_page");
  setMinimumSize(QSize(300, 150));
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

#ifndef ANTIQUA_DEVELOPEMENT
  btn_cancel->setEnabled(false);
  btn_cancel->setToolTip(tr("Currently not supported!"));
#endif

  setLayout(layout);

  connect(btn_status, SIGNAL(clicked()), this, SLOT(statusClicked()));
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void ABE_StartPage::statusClicked() { emit sendGotoPage(1); }
void ABE_StartPage::cancelClicked() {
  // emit sendGotoPage(2);
}

AbeBooksRemoteActions::AbeBooksRemoteActions(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setWindowTitle("AbeBooks [*]");
  setContentsMargins(5, 0, 5, 10);

  m_requester = new AbeBooksRequester(this);
  m_requester->setObjectName("abebooks_remote_actions");

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
  m_startPage = new ABE_StartPage(m_stackedWidget);
  m_stackedWidget->insertWidget(page++, m_startPage);
  m_statusPage = new ABE_StatusPage(m_stackedWidget);
  m_stackedWidget->insertWidget(page++, m_statusPage);
  // END

  // SIGNALS
  connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
          SLOT(setCurrentIndex(int)));

  connect(m_statusPage, SIGNAL(sendAction(const QString &)), this,
          SLOT(prepareStatusAction(const QString &)));
  connect(m_statusPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));

  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void AbeBooksRemoteActions::prepareStatusAction(const QString &cmd) {
  qDebug() << Q_FUNC_INFO << cmd;
}

void AbeBooksRemoteActions::pushMessage(const QString &msg) {
  qDebug() << Q_FUNC_INFO << msg;
}

void AbeBooksRemoteActions::setPurchaser(const QString &person) {
  if (person.isEmpty())
    return;

  p_purchaser = person;
}

void AbeBooksRemoteActions::setEMail(const QString &email) {
  if (email.isEmpty())
    return;

  p_purchaser_mail = email;
}

int AbeBooksRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
