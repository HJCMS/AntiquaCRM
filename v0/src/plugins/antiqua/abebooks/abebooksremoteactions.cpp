// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksremoteactions.h"
#include "abebooksconfig.h"
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
#ifndef ANTIQUA_DEVELOPEMENT
  setEnabled(false);
#endif
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
  m_statusPage = new ABE_StatusPage(this);
  layout->addWidget(m_statusPage);
  layout->setStretch(0, 1);
  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_buttonBar);
  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);
  // END

  // SIGNALS
  connect(m_statusPage, SIGNAL(sendAction(const QString &)), this,
          SLOT(prepareStatusAction(const QString &)));
  connect(m_statusPage, SIGNAL(sendNotes(const QString &)), this,
          SLOT(pushMessage(const QString &)));

  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void AbeBooksRemoteActions::prepareStatusAction(const QString &cmd) {
  if (p_articles.count() < 1) {
    qWarning("Missing Article Ids!");
    return;
  }

  if (p_orderId.isEmpty()) {
    qWarning("Missing Order Id!");
    return;
  }

  AbeBooksRequester *req = new AbeBooksRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QDomDocument &)), this,
          SLOT(responseUpdate(const QDomDocument &)));

  req->updateOrderStatus(p_orderId, cmd);
}

void AbeBooksRemoteActions::pushMessage(const QString &msg) {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << msg;
#endif
}

void AbeBooksRemoteActions::responseUpdate(const QDomDocument &dom) {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl;
  saveSources(dom, "update_" + p_orderId);
#endif
}

void AbeBooksRemoteActions::setPurchaser(const QString &person) {
  if (person.isEmpty())
    return;

  p_purchaser = person;
}

void AbeBooksRemoteActions::setArticleIds(const QStringList &ids) {
  if (ids.count() < 1)
    return;

  p_articles = ids;
}

int AbeBooksRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
