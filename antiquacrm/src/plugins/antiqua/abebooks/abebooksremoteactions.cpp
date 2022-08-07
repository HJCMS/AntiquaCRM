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

  setLayout(layout);

  connect(btn_status, SIGNAL(clicked()), this, SLOT(statusClicked()));
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void ABE_StartPage::statusClicked() {}
void ABE_StartPage::cancelClicked() {}

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

  // END

  // SIGNALS
  // connect(m_startPage, SIGNAL(sendGotoPage(int)), m_stackedWidget,
  // SLOT(setCurrentIndex(int)));
  connect(m_buttonBar, SIGNAL(rejected()), this, SLOT(reject()));
  // END
}

void AbeBooksRemoteActions::prepareAction(const QJsonObject &jsObj) {}

void AbeBooksRemoteActions::pushMessage(const QString &msg) {}

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
